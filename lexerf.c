#include "lexerf.h"

void print_token(Token token){
  printf("TOKEN VALUE: ");
  for(int i = 0; token.value[i] != '\0'; i++){
    printf("%c", token.value[i]);
  }
  if(token.type == INT){
    printf(" TOKEN TYPE: INT\n");
  }
  if(token.type == KEYWORD){
    printf(" TOKEN TYPE: KEYWORD\n");
  }
  if(token.type == SEPARATOR){
    printf(" TOKEN TYPE: SEPARATOR\n");
  }
}

Token *generate_number(char *current, int *current_index){
  Token *token = malloc(sizeof(Token));
  token->type = INT;
  char *value = malloc(sizeof(char) * 16);
  int value_index = 0;
  while(isdigit(current[*current_index]) && current[*current_index] != '\0'){
    value[value_index] = current[*current_index];
    value_index++;
    *current_index += 1;
  }
  value[value_index] = '\0';
  token->value = value;
  return token;
}

Token *generate_keyword(char *current, int *current_index){
  Token *token = malloc(sizeof(Token));
  char *keyword = malloc(sizeof(char) * 16);
  int keyword_index = 0;
  while(isalpha(current[*current_index]) && current[*current_index] != '\0'){
    keyword[keyword_index] = current[*current_index];
    keyword_index++;
    *current_index += 1;
  }
  keyword[keyword_index] = '\0';
  if(strcmp(keyword, "exit") == 0){
    token->type = KEYWORD;
    token->value = "EXIT";
  } else {
    token->type = KEYWORD;
    token->value = keyword;
  }
  return token;
}

size_t tokens_index;

Token *lexer(FILE *file){
  int length;
  char *buffer = 0;
  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);
  buffer = malloc(sizeof(char) * (length + 1));
  fread(buffer, 1, length, file);
  buffer[length] = '\0';
  
  int current_index = 0;
  Token *tokens = malloc(sizeof(Token) * 1024);
  tokens_index = 0;

  while(buffer[current_index] != '\0'){
    if(isspace(buffer[current_index])){
      current_index++;
      continue;
    }
    
    Token *token = malloc(sizeof(Token));
    
    if(buffer[current_index] == ';'){
      token->value = malloc(2);
      token->value[0] = buffer[current_index];
      token->value[1] = '\0';
      token->type = SEPARATOR;
      tokens[tokens_index] = *token;
      tokens_index++;
      current_index++;
    } else if(buffer[current_index] == '('){
      token->value = malloc(2);
      token->value[0] = buffer[current_index];
      token->value[1] = '\0';
      token->type = SEPARATOR;
      tokens[tokens_index] = *token;
      tokens_index++;
      current_index++;
    } else if(buffer[current_index] == ')'){
      token->value = malloc(2);
      token->value[0] = buffer[current_index];
      token->value[1] = '\0';
      token->type = SEPARATOR;
      tokens[tokens_index] = *token;
      tokens_index++;
      current_index++;
    } else if(isdigit(buffer[current_index])){
      token = generate_number(buffer, &current_index); 
      tokens[tokens_index] = *token;
      tokens_index++;
    } else if(isalpha(buffer[current_index])){
      token = generate_keyword(buffer, &current_index);
      tokens[tokens_index] = *token;
      tokens_index++;
    } else {
      current_index++;
    }

    free(token);
  }
  
  tokens[tokens_index].value = NULL;
  tokens[tokens_index].type = END_OF_TOKENS;
  
  free(buffer);
  return tokens;
}
