#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの型を表す値
enum {
      TK_NUM = 256, // 整数トークン
      TK_IDENT,     // 識別子
      TK_EOF,       // 入力の終わりを表すトークン
};

// トークンの型
typedef struct {
  int ty;      // トークンの型
  int val;     // tyがTK_NUMの場合、その数値
  char *input; // トークン文字列（エラーメッセージ用）
} Token;

enum {
      ND_NUM = 256,
      ND_IDENT,
};

typedef struct Node {
  int ty;
  struct Node *lhs;
  struct Node *rhs;
  int val;
  char *name;
} Node;

// トークナイズした結果のトークン列はこの配列に保存する
// 100個以上のトークンは来ないものとする
extern Token tokens[100];
extern int pos;

// パースした式をこの配列に保存する
extern Node *code[100];
extern int stmt;

void tokenize(char *p);
Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_ident(char *input);
void program();
Node *assign();
Node *expr();
Node *mul();
Node *term();
void gen(Node *node);
