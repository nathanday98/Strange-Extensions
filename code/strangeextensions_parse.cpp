static bool
IsEndOfFuncionCall(char *C)
{
    return (*C == '(');
}

static bool
IsEndOfDefine(char *C)
{
    return (*C == ',' 
        || *C == ')' 
        || *C == ' ' 
        || *C == ';' 
        || *C == '|');
}

static bool 
IsEndOfLine(char *C)
{
    return (*C == '\n'
        || *C == '\r'
        || *C == '\0'
        || *C == 0);
}

static bool
StringCompare(char *A,
              char *B,
              u32 Len)
{
    for(int Index = 0; 
        Index < Len; 
        Index++)
    {
        if(*(A + Index) != *(B + Index))
            return false;
    }
    return true;
}

static char *
IsInCoreARB(char *Function,
            u32 FunctionLen,
            file_data *CoreARB)
{
    for(int Character = 0; 
        Character < CoreARB->ContentSize; 
        Character++)
    {
        char *C = CoreARB->Contents + Character;
        if(StringCompare(Function, C, 
                         FunctionLen)
            && *(C + FunctionLen) == ' ')
        {
            return C;
        }
    }

    return 0;
}

/* 
    NOTE(nathan): passing text instead of file data because we could add
                  reading multiple code blocks asynchronously if needed

*/
static void
ParseText(char *Input, 
          u32 InputSize,
          file_data *CoreARBFile)
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
            u32 Len = (u32)(End - C)-1;
            char *Location = IsInCoreARB(C, 
                                         Len, 
                                         CoreARBFile);

            if(Location)
            {
                // printf("%.*s\n", Len, Location);
                char *LocationWalker = Location + Len;
                while(*LocationWalker == ' ')
                {
                    LocationWalker++;
                }

                char *ValueWalker = LocationWalker;
                while(!IsEndOfLine(ValueWalker))
                {
                    ValueWalker++;
                }
                u32 ValueLen = (u32)(ValueWalker - LocationWalker);
                printf("%.*s %.*s\n", Len, Location, ValueLen, LocationWalker);
            }
        }

        if(*C == 'g' 
            && *(C+1) == 'l')
        {
            char *End = C;
            while(!IsEndOfFuncionCall(End++))
            {

            }
            u32 Len = (u32)(End - C)-1;
            char *Location = IsInCoreARB(C, Len, CoreARBFile);
            if(Location)
            {
                char *LocationWalker = Location + Len;
                while(*LocationWalker == ' ')
                {
                    LocationWalker++;
                }

                char *ValueWalker = LocationWalker;
                while(!IsEndOfLine(ValueWalker))
                {
                    ValueWalker++;
                }

                u32 ValueLen = (u32)(ValueWalker - LocationWalker);
                printf("%.*s %.*s\n", Len, Location, ValueLen, LocationWalker);
            }
        }
    }
}