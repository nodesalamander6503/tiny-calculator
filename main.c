#define VERSION "v1.0"

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define error(fmt, ...) do { fprintf(stderr, "error: " fmt "\n", ##__VA_ARGS__); exit(1); } while (0)

struct sexpr {
	bool is_call;
	union {
		struct {
			char * name;
			unsigned int num_args;
			struct sexpr ** args;
		} call;
		float num;
	} data;
};

enum fn_t {
	t_fold,
	t_bin,
	t_unary,
};
struct function {
	const char * name;
	enum fn_t t;
	union {
		float(*fold)(float, float);
		float(*bin)(float, float);
		float(*unary)(float);
	} fn;
};
float fn_add(float x, float y) { return x + y; }
float fn_sub(float x, float y) { return x - y; }
float fn_mul(float x, float y) { return x * y; }
float fn_div(float x, float y) { return x / y; }
float fn_fv(float i, float n) { return pow(1 + i, n); }
float fn_pv(float i, float n) { return pow(1 + i, -n); }
float fn_ai(float i, float n) { return (1 - pow(1 + i, -n)) / i; }
float fn_ad(float i, float n) { return ((1 - pow(1 + i, -n)) / i) * (1 + i); }
float fn_si(float i, float n) { return (pow(1 + i, n) - 1) / i; }
float fn_sd(float i, float n) { return ((pow(1 + i, n) - 1) / i) * (1 + i); }
float fn_perp(float i) { return 1 / i; }
struct function functions[] = {
	{"add", t_fold, {.fold = fn_add}},
	{"sub", t_fold, {.fold = fn_sub}},
	{"mul", t_fold, {.fold = fn_mul}},
	{"div", t_fold, {.fold = fn_div}},

	{"fv", t_bin, {.bin = fn_fv}},
	{"pv", t_bin, {.bin = fn_pv}},
	{"ai", t_bin, {.bin = fn_ai}},
	{"ad", t_bin, {.bin = fn_ad}},
	{"si", t_bin, {.bin = fn_si}},
	{"sd", t_bin, {.bin = fn_sd}},

	{"perp", t_unary, {.unary = fn_perp}},

	{NULL, 0, NULL} // sentinel
};

bool faststrneq(const char * x, const char * y) {
	int i = 0;
	for(; x[i] && x[i] == y[i]; i ++);
	return (x[i] != 0);
}

struct sexpr * eval_sexpr(struct sexpr * s) {
	if(!(s->is_call)) {
		return s;
	}
	if(s->data.call.num_args == 0) {
		error("need to have at least 1 argument");
	}
	for(int i = 0; i < s->data.call.num_args; i ++) {
		s->data.call.args[i] = eval_sexpr(s->data.call.args[i]);
	}
	float n = 0;
	for(int i = 0; functions[i].name; i ++) {
		if(faststrneq(s->data.call.name, functions[i].name)) { continue; }
		if(functions[i].t == t_fold) {
			n = s->data.call.args[0]->data.num;
			for(int j = 1; j < s->data.call.num_args; j ++) {
				n = functions[i].fn.fold(n, s->data.call.args[j]->data.num);
			}
		} else if(functions[i].t == t_bin) {
			if(s->data.call.num_args != 2) { error("the function %s expects exactly 2 arguments", s->data.call.name); }
			n = functions[i].fn.bin(s->data.call.args[0]->data.num, s->data.call.args[1]->data.num);
		} else if(functions[i].t == t_unary) {
			if(s->data.call.num_args != 1) { error("the function %s expects exactly 1 argument", s->data.call.name); }
			n = functions[i].fn.unary(s->data.call.args[0]->data.num);
		} else {
			error("implementation bug: the code used to implement the function %s was malformed", s->data.call.name);
		}
		goto solved;
	}
	error("could not find instruction %s", s->data.call.name);
solved:
	// first, free everything rlly quick
	for(int j = 1; j < s->data.call.num_args; j ++) {
		free(s->data.call.args[j]);
	}
	free(s->data.call.args);
	// now actually return value
	s->is_call = 0;
	s->data.num = n;
	return s;
};

static struct sexpr *parse_expr(const char **p);
static void skip_ws(const char **p) {
	while (isspace(**p)) ++(*p);
}
static char * parse_ident(const char ** p) {
	const char * start = * p;
	while (isalnum(**p) || **p=='_' || **p=='-') ++(*p);
	size_t len = *p - start;
	if (len == 0) error("expected identifier");
	char * s = malloc(len + 1);
	memcpy(s, start, len); s[len] = '\0';
	return s;
}
static struct sexpr *parse_number(const char **p) {
	char *end;
	double v = strtod(*p, &end);
	if (end == *p) error("bad number");
	*p = end;
	struct sexpr *n = malloc(sizeof *n);
	n->is_call = false;
	n->data.num = (float)v;
	return n;
}
static struct sexpr *parse_expr(const char **p) {
	skip_ws(p);
	if (**p == '(') {
		++(*p); skip_ws(p);
		char *name = parse_ident(p);
		size_t cap = 4, sz = 0;
		struct sexpr **args = malloc(cap * sizeof *args);

		while (1) {
			skip_ws(p);
			if (**p == ')') { ++(*p); break; }
			if (sz == cap) { cap *= 2; args = realloc(args, cap * sizeof *args); }
			args[sz++] = parse_expr(p);
		}
		struct sexpr *node = malloc(sizeof *node);
		node->is_call = true;
		node->data.call.name = name;
		node->data.call.num_args = (unsigned)sz;
		node->data.call.args = args;
		return node;
	}
	return parse_number(p);
}
struct sexpr *sexpr_from_string(const char *src) {
	const char *p = src;
	struct sexpr *root = parse_expr(&p);
	skip_ws(&p);
	if (*p != '\0')
		error("unexpected trailing characters");
	return root;
}

void print_float(float xf) {
	char buf[32];
	snprintf(buf, sizeof buf, "%.7f", (double)xf);
	char *p = buf + strlen(buf) - 1;
	while (p > buf && *p == '0') { --p; }
	if (*p == '.') {
		*++p = '0';
	}
	*(p + 1) = '\0';
	printf("%s", buf);
}

char * input_buffer = 0;
unsigned int input_buffer_size = 0;
char * get_input(void) {
	if(input_buffer == 0) {
		input_buffer_size = 64;
		input_buffer = malloc(input_buffer_size);
	}
	printf("> ");
	fflush(stdout);
	char c;
	unsigned int index = 0;
	while(true) {
		c = getchar();
		if(c == EOF || c == '\n') {
			break;
		}
		if(index == input_buffer_size - 1) {
			input_buffer_size *= 2;
			input_buffer = realloc(input_buffer, input_buffer_size);
		}
		input_buffer[index] = c;
		index ++;
	}
	input_buffer[index] = 0;
	return input_buffer;
}

int main(int argc, char ** argv) {
	bool is_repl = true;
	char * input = 0;
	if(argc > 1) {
		input = argv[1];
		is_repl = false;
	}
	if(is_repl) {
		printf("tiny calculator " VERSION "\ncreated with C by node™\n");
	}
	do {
		if(is_repl) { input = get_input(); }
		struct sexpr * tree = sexpr_from_string(input);
		struct sexpr * res  = eval_sexpr(tree);
		print_float(res->data.num);
		printf("\n");
	} while(is_repl);
	if(is_repl) {
		printf("exiting REPL\n");
	}
}





