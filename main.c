#include<stdio.h>
#include<string.h>
#include <ctype.h> 
#include <stdlib.h>
#define RED "\x1b[31m"
#define PINK "\x1b[35m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RESET  "\x1b[0m"
char *keyword[]={"int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof","struct"};
static const char* operators = "+-*,/%=!<>|&(){}[];";
int is_keyword(unsigned char key[]){
    for(int i=0;i<sizeof(operators)/sizeof(char *);i++){
      if(strcmp(key,keyword[i])==0){
        return 1;
      }
    }
    return 0;

}
int is_operator(unsigned char op){
    
    strchr(operators,op)!=NULL ;      
}

void number_type(char type[]){
  
   if(type[0]=='0' && isdigit(type[1])){
      if(type[1]<='7' && type[2]<= '7' && type[3] <=  '7' && strlen(type)<=4 )
        printf(GREEN"Octal :"RESET"  %s\n",type);
      else
        printf(RED"Not an octal number\n");
   }
   else if(type[1]=='x' || type[1]=='X'){
      if((type[2] <='f'|| type[2]<='F') && (type[3]<='f'|| type[3]<='F') && (type[4]<='f'|| type[4]<='F') && (type[5]<='f'|| type[5]<='F'))
         printf(GREEN"Hexadecimal :"RESET"  %s\n",type);
      else
         printf(RED"Not an hexadecimal number\n");
    }
    else if( type[0] == '0' && (type[1] == 'b'|| type [1] == 'B')){
      int flag=0;
      for(int i=2; type[i]!='\0';i++){
        if(type[i]!='0' || type[i] != '1'){
          flag=1;
          break;
        }
      }
      if(flag==0)
           printf(GREEN"Binary :"RESET"  %s\n",type);
        else{
          printf(RED"Not an binary number\n");
          exit(0);
        }
    }
    else if(strchr(type,'.')!=NULL){
    int count_d=0;
    int count_f=0;
    int count_a_z=0;
    int i=0;
    int n=strlen(type);
    while(type[i]!='\0'){
      if(((type[i]>='a' && type[i]<'f') || (type[i]>='g' && type[i]<= 'z'))){
        count_a_z++;
      }
       if(type[i]=='f'){
        count_f++;
         
       }
      if(type[i]=='.'){
        count_d++;
      }
      i++;
    }
    if( count_d >=2 || count_f >=2 || count_a_z >=1 ){
          printf("Not an float number\n");
           exit(0);
    }
   
    else if( (strchr(type,'f')!=NULL) ){
       printf(GREEN"Float :"RESET"  %s\n",type);
    }
    else if((type[n-1]>='a' && type[n-1]<'f') || (type[n-1]>='g' && type[n-1]<= 'z') ){
          printf("Not an float number\n");
           exit(0);
    }
    else if((strchr(type,'.')!=NULL)  ){
      printf(GREEN"Float :"RESET"  %s\n",type);
    }
   }
    else if(isdigit(type[0]) && isdigit(type[1]) ){
      printf(GREEN"Numeric constant :"RESET"  %s\n",type);
    }
    else{
      printf(RED"Not an valid number\n");
      exit(0);
    }
}
int main(){
    FILE *fptr=fopen("eg.c","r");
    int num;
    char buffer[200];
    int i=0;
    char ch;
    while((ch=fgetc(fptr))!=EOF)
    {
        if(isalpha(ch)){
           buffer[i++]=ch; 
            while((ch=fgetc(fptr))!=EOF && isalnum(ch)|| ch=='_'){
              buffer[i++]=ch;
            }
            buffer[i]='\0';
            i=0;
            if(is_keyword(buffer)==1){
                printf(RED"Keyword:"RESET"%s\n",buffer);

            }
            else{
                printf(PINK"Identifier:"RESET" %s\n",buffer);
            }
            ungetc(ch,fptr);// if we read non alphanumeric character then , put it back for  further processing
        }
        else if(isdigit(ch)){
            buffer[i++]=ch; 
            // if(ch>='0' && ch<='9'){
             
            // while((ch=fgetc(fptr))!=EOF && ch!='='){
            //   buffer[i++]=ch;
            // }
            // buffer[i]='\0'; 
            // i=0;
            // if((buffer[1]>='a' && buffer[1]<'b') || (buffer[1]>='c' && buffer[1]<'x') || (buffer[1]=='y' && buffer[1]=='z')){
            //   printf(RED"Not an identifier %s",buffer);
            //   exit(0);
            // }
            // }
            // else{
            while((ch=fgetc(fptr))!=EOF && ch!=';'){
              buffer[i++]=ch;
            }
            buffer[i]='\0'; 
            i=0;
            number_type(buffer);
            //}
            ungetc(ch,fptr);// if we read non alphanumeric digit then , put it back for  further processing
        }
        else if(ch=='/'){
          char ch1=fgetc(fptr);
          if(ch1 == '*'){
            while(( (ch=fgetc(fptr))!='/')  && ch !=EOF);
          }
          else if( ch1 =='/' ){
            while(( ch=fgetc(fptr)!='\n' ) && ch !=EOF);
          }
          else{
            ungetc(ch1,fptr);
            printf(BLUE"operator: "RESET" %s\n",buffer);
          }
        }
        else if(ch=='\'' || ch== '"'){
          char quote=ch;
          char ch1;
          int i=0;
          while((ch1=fgetc(fptr))!=EOF){
            if(ch1 == quote){
              buffer[i]='\0';
              if(quote == '\''){
                 printf(PINK"character constant : "RESET"'%s' \n",buffer);
              }
              else{
                printf(PINK"Literal : "RESET"\"%s\"\n",buffer);
              }
              break;
            }
            else if( ch1 == ';' || ch1 == EOF ){
              printf(RED"ERROR: Missing closing %c for literal\n",quote);
              exit(0);
            }
            else if(quote == '\'' && i==0){
              buffer[i++]=ch1;
            }
            else if(quote == '"'){
              buffer[i++]=ch1;
            }
           
            
          }
            
          
        }
        else if(is_operator(ch)){
            buffer[i++]=ch; 
            while((ch=fgetc(fptr))!=EOF && is_operator(ch)){
              buffer[i++]=ch;
            }
            buffer[i]='\0';
            i=0;
            printf(BLUE"operator: "RESET" %s\n",buffer);
        }
        
    }
}