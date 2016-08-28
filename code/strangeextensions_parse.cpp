static bool
IsEndOfDefine(char *C)
{
    return (*C == ',' 
        || *C == ')' 
        || *C == ' ' 
        || *C == ';' 
        || *C == '|');
}

/* 
    NOTE(nathan): passing text instead of file data because we could add
                  reading multiple code blocks asynchronously if needed

*/
static void
ParseText(char *Input, 
          u32 InputSize)
{   
    char *C = Input;
    while(*(C++))
    {
        if(*C == 'G' 
            && *(C+1) == 'L' 
            && *(C+2) == '_')
        {
            char *End = C;
            while(!IsEndOfDefine(End++))
            {

            }
            u32 Len = (size_t)(End - C)-1;
            printf("%.*s\n", 
                    Len, 
                    C);
        }
    }
}