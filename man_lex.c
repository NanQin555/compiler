//王南钦 2022091605017 
//2023.9.11

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int t = -1;//种别码
int pProject = 0;//指针

//reservedword
static char reserveWord[14][10] =
{
    "break", "char", "const", "continue",
    "else","float"
    "for", "if", "else", "int", "float", "return",
    "void","while"
};


//boredrword
static char boredrWord[31][10] =
{
    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
    "!=", ";", "(", ")", ",", "\"", "\'", "&",
    "&&", "|", "||", "%", "<<", ">>", "[", "]", "{",
    "}", ".", ":", "!"
};

//identifier
static char identifier[1000][50] = {""};


int filtrationResource( char resourceProject[] , int end )
{
    char temp[10000];
    int count = 0;
    int i;
    for( i = 0 ; i < end ; i++ )
    {
        //head 
        //此处printf函数输出有问题，结果；应为(  head  ,  <stdio.h>  ）在为windows下确实是如此，在另两台linux下，一个正确一个错误
        //但在Linux下输出是  ）head,  <stdio.h>     
        //怀疑和Linux printf 下的缓冲区有问题
        if ( resourceProject[i] == '#' && resourceProject[i + 1] == 'i' && resourceProject[i + 2] == 'n' && resourceProject[i + 3] == 'c' 
        && resourceProject[i + 4] == 'l' && resourceProject[i + 5] == 'u' && resourceProject[i + 6] == 'd' && resourceProject[i + 7] == 'e' )
        {

            printf("(  head  ,  ");
            fflush(stdout);
            while( resourceProject[i] !='<')
            {
                i++;
            }
            while( resourceProject[i] != '\n' )
            {

                printf("%c",resourceProject[i++]);
                fflush(stdout);
            }

            printf("  )\n");
            fflush(stdout);
        } 
         

        //删除单行注释
        if ( resourceProject[i] == '/' && resourceProject[i + 1] == '/')
        {
            while( resourceProject[i] != '\n' )
            {
                i++;
            }
        }

        //删除多行注释
        if ( resourceProject[i] == '/' && resourceProject[i + 1] == '*')
        {
            i = i + 2;
            while( resourceProject[i] != '*' && resourceProject[i + 1] != '/')
            {
                i++;
                if( i == end )
                {
                    break;
                }
            }
            i = i + 2;
        }

        //删除换行符 制表符
		if ( resourceProject[i] != '\n' && resourceProject[i] != '\t' && resourceProject[i] != '\r')
		{
			temp[count] = resourceProject[i];
			count++;
		}
    }
    temp[count] ='\0';//末位增加结束符
    strcpy( resourceProject , temp );
    return count;//更新当前源程序数组的末尾指针
}

//判断是否为数字
int IsDigit(char digit)
{
    if (digit >= '0'&&digit <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//判断是否为字母
int IsLetter(char letter)
{
    if ( (letter >= 'a'&&letter <= 'z') || (letter >= 'A'&&letter <= 'Z') || letter=='_')
    {
        return 1;
    }
    else
    {
       return 0;
    }
}

//查找reservedword
int searchReserve(char reserveWord[][10], char s[])
{
    int i;
    for ( i = 0; i < 32; i++ )
    {
        if (strcmp(reserveWord[i], s) == 0)
        {    //若成功查找，则返回种别码
            i++;
            return i;//返回种别码
        }
    }
    return -1;

}

int Scaning( char resourceProject[] , char token[] )
{
    int i, count = 0;
    char ch;
    ch = resourceProject[pProject];
 
    while (ch ==' ')
    {//空格
        pProject++;
        ch = resourceProject[pProject];
    }

    for (i = 0; i<20; i++)
    {//每次清零
        token[i] = '\0';
    }

    if (IsLetter(resourceProject[pProject]))
    {//开头为字母
        token[count] = resourceProject[pProject];//收集
        count++;
        pProject++;//下移
        while (IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject]))
        {//后跟字母或数字
            token[count++] = resourceProject[pProject];
            pProject++;
        }
        
        token[count] = '\0';
        t =  searchReserve(reserveWord, token);//查表找到种别码
        if (t == -1)
        {//若不是reservedword则是identifier
             t = 100;//identifier种别码
        }    
        return t;
    }
    else if (IsDigit(resourceProject[pProject]))
    {//首字符为数字
        while (IsDigit(resourceProject[pProject]))
        {//后跟数字
            token[count] = resourceProject[pProject];//收集
            count++;
            pProject++;
        }//多读了一个字符既是下次将要开始的指针位置
        token[count] = '\0';
        t = 100;
        return t;
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')'
                || ch == ',' || ch == '\"' || ch == '\'' || ch == '%' || ch == '['
                || ch == ']' || ch == '{' || ch == '}' || ch == '.' || ch == ':')
    {//若为运算符或者boredrword，查表得到结果
        token[0] = resourceProject[pProject];
        token[1] = '\0';//形成单字符串
        for (i = 0; i<32; i++)
            {//查运算符boredrword表
                if (strcmp(token, boredrWord[i]) == 0)
                {
                    t = 15 + i;
                    break;
                }
            }
        pProject++;//指针下移，为下一扫描做准备
        return t;
    }
    else  if (resourceProject[pProject] == '<')
    {//        <,<=,<<
        pProject++;//后移，超前搜索
         if (resourceProject[pProject] == '=')
        {
            t = 20;
        }
        else if (resourceProject[pProject] == '<')
        {//左移
            pProject--;
            t = 37;
        }
        else
        {
            pProject--;
            t = 19;
        }
        pProject++;//指针下移
        return t;
    }
    else  if (resourceProject[pProject] == '>')
    {//        >,>=,>>
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            t = 22;
        }
        else if (resourceProject[pProject] == '>')
        {
            t = 38;
        }
        else
        {
            pProject--;
            t = 21;
        }
        pProject++;
        return t;
    }
    else  if (resourceProject[pProject] == '=')
    {//        =,==
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            t = 24;
        }
        else
        {
            pProject--;
            t = 23;
        }
        pProject++;
        return t;
    }
    else  if (resourceProject[pProject] == '!')
    {//        !,!=
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            t = 25;
        }
        else
        {
            t = 45;
            pProject--;
        }
        pProject++;
        return t;
    }
    else  if (resourceProject[pProject] == '&')
    {//        &,&&
        pProject++;
        if (resourceProject[pProject] == '&')
        {
            t = 32;
        }
        else
        {
            pProject--;
            t = 33;
        }
        pProject++;
        return t;
    }
    else  if (resourceProject[pProject] == '|')
    {//        |,||
        pProject++;
        if (resourceProject[pProject] == '|')
        {
            t = 35;
        }
        else
        {
            pProject--;
            t = 34;
        }
        pProject++;
        return t;
    }
    else  if (resourceProject[pProject] == '\0')
    {//结束符
        t = 0;//种别码为0
        return t;
    }
    else
    {//不能被以上词法分析识别，则出错。
        printf("error:there is not exist %c00 \n", ch);
        exit(0);
    }
}
int main()
{
    //读取源程序
    char resourceProject[10000];
    char token[20] = {0};
    int i;
    int end = 0;//数组末尾下标
    FILE *fp;
    if ( ( fp = fopen ( "test.c", "r" ) ) == NULL )
    {
        printf("No such file!\n");
        return 0;
    }

    //将源程序读入resourceProject数组
    resourceProject[ pProject ] = fgetc(fp);

    while(resourceProject[ pProject ] != EOF )
    {
        pProject++;
        resourceProject[ pProject ] = fgetc(fp);
    }
    fclose(fp);
    end = filtrationResource( resourceProject , pProject );
    pProject = 0;

    while (t != 0)
    {
        //启动扫描
        t = Scaning(resourceProject, token);
        if (t == 100)
        {
            for (i = 0; i<1000; i++)
            {//插入identifier表中
                if (strcmp(identifier[i], token) == 0)
                {//已在表中
                    break;
                }
                if (strcmp(identifier[i], "") == 0)
                {//查找空间
                    strcpy(identifier[i], token);
                    break;
                }
            }
            printf("(  identifier  ,  %s  )\n", token);
        }
        else if (t >= 1 && t <= 14)
        {
            printf("(  reservedword  ,  %s  )\n", reserveWord[t - 1]);
        }
        else if (t == 100)
        {
            printf("(  num  ,  %s  )\n", token);
        }
        else if (t >= 15 && t <= 45)
        {
            printf("(  boredrword  ,  %s  )\n", boredrWord[t - 15]);
        }
    }
}
