#include "9cc.h"

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

Node *new_node_ident(char *input) {
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

Node *assign() {
  Node *lhs = expr();
  
  if (tokens[pos].ty == ';') {
    pos++;
    return lhs;
  }
  if(tokens[pos].ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }
  fprintf(stderr, "セミコロンがありません:%s\n",tokens[pos].input);
}

Node *expr() {
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

Node *mul() {
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

Node *term() {
  if (tokens[pos].ty == TK_NUM)
    return new_node_num(tokens[pos++].val);
  if (tokens[pos].ty == TK_IDENT)
    return new_node_ident(tokens[pos++].input);
  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();
    if (tokens[pos].ty != ')') 
      fprintf(stderr, "開きカッコに対応する閉じカッコがありません: %s",
	    tokens[pos].input);
    pos++;
    return node;
  }
  fprintf(stderr,"数値でも開きカッコでもないトークンです: %s" ,
	  tokens[pos].input);
  
}

