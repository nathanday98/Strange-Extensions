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

static bool
HasBeenParsed(u64 Function,
              u64 *CheckBuffer,
              u32 CheckBufferSize)
{
    for(u32 Index = 0;
        Index < CheckBufferSize+1; // NOTE(nathan): if this is 0 then it wont scan on first attempt, so plus 1
        Index++)
    {
        u64 ToCheck = *(CheckBuffer + Index);
        if(Function == ToCheck)
        {
            return true;
        }
    }
    return false;
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
    u32 CheckBufferSize = 0;
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
                if(!HasBeenParsed((u64)Location, (u64 *)Input, CheckBufferSize))
                {
                    CheckBufferSize++;
                    printf("%.*s %.*s\n", Len, Location, ValueLen, LocationWalker);
                    *((u64 *)Input + CheckBufferSize-1) = (u64)Location;
                }
            }
        }

        // // memset(Input, 0, CheckBufferSize * sizeof(u64));

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

                char *ReturnTypeWalker = Location - 10;
                u32 ReturnTypeLen = 0;
                while(!StringCompare(ReturnTypeWalker - 5, "GLAPI", 5))
                {
                    ReturnTypeLen++;
                    ReturnTypeWalker--;
                }
                ReturnTypeWalker++;
                ReturnTypeLen--;


                u32 ValueLen = (u32)(ValueWalker - LocationWalker);
                if(!HasBeenParsed((u64)Location, (u64 *)Input, CheckBufferSize))
                {
                    CheckBufferSize++;
                    printf("%.*s %.*s\n", ReturnTypeLen, ReturnTypeWalker, Len, Location);
                    *((u64 *)Input + CheckBufferSize-1) = (u64)Location;                    
                }
            }
        }

        if(CheckBufferSize * 4 > C - Input)
        {
            printf("Check buffer is overflowing\n");
        }

    }

    printf("Check buffer size: %d\n", CheckBufferSize);
}