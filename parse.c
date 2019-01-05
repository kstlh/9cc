#include "9cc.h"

static Node *new_node(int ty, Node *lhs, Node *rhs);
static Node *new_node_num(int val);
static Node *new_node_ident(char *input);
static Node *assign();
static Node *expr();
static Node *mul();
static Node *term();

static Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

static Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

static Node *new_node_ident(char *input) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_IDENT;
  node->name = input;
  return node;
}

void program() {
  while(tokens[pos].ty != TK_EOF) {
    code[stmt++] = assign();
  }
}

static Node *assign() {
  Node *lhs = expr();
  
  if (tokens[pos].ty == ';') {
    pos++;
    return lhs;
  }
  if(tokens[pos].ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }
  error("セミコロンがありません:%s\n",tokens[pos].input);
}

static Node *expr() {
  Node *lhs = mul();
  if (tokens[pos].ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }
  if (tokens[pos].ty == '-') {
    pos++;
    return new_node('-', lhs, expr());
  }
  return lhs;
}

static Node *mul() {
  Node *lhs = term();
  if (tokens[pos].ty == '*') {
    pos++;
    return new_node('*', lhs, mul());
  }
  if (tokens[pos].ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }
  return lhs;
}

static Node *term() {
  if (tokens[pos].ty == TK_NUM)
    return new_node_num(tokens[pos++].val);
  if (tokens[pos].ty == TK_IDENT)
    return new_node_ident(tokens[pos++].input);
  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();
    if (tokens[pos].ty != ')') 
      error("開きカッコに対応する閉じカッコがありません: %s",
	    tokens[pos].input);
    pos++;
    return node;
  }
  error("数値でも開きカッコでもないトークンです: %s" ,
	  tokens[pos].input);
  
}

