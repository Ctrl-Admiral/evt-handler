#include "evt-functions.h"

void printUsage(char **argv)
{
    printf("Usage: %s filename [-hi]\n", argv[0]);
    printf("\t filename: the event log (EVT) to handle\n");
    printf("\t-h:   shows this help\n");
    printf("\t-i:   shows only base info about log without info of every record\n");
}


int main(int argc, char **argv)
{
    Status status = StatusError;

    FILE*   file = NULL;
    size_t fileSize = 0;
    BYTE* startFile = NULL;

    bool hasHeader = false;
    bool hasFooter = false;
    bool isFileWrapped = false;
    BYTE* startFooter = NULL;

    EventLog eventLog;
    int numArgc = 3;
    bool baseMode = false;

    printf("size of DWORD_UNDEF: %zu\n", sizeof(UNDEF_DWORD));
    printf("size of DWORD: %zu\n", sizeof(DWORD));
    printf("size of WORD_UNDEF: %zu\n", sizeof(UNDEF_WORD));
    printf("size of WORD: %zu\n", sizeof(WORD));

    if (argc > numArgc)
    {
        printf("Please specify filename and correct mode of output.\n");
        printUsage(argv);
        goto out0;
    }

    if (argc == numArgc)
    {
        if(strcmp(argv[2], "-h") == 0)
        {
            printUsage(argv);
            goto out0;
        }
        else if (strcmp(argv[2], "-i") == 0)
        {
            baseMode = true;
        }
        else
        {
            printf("Unknown input parameter.\n");
            printUsage(argv);
            goto out0;
        }
    }

    file = fopen(argv[1], "rb");
    if (fseek(file, 0, SEEK_END))
    {
        fprintf(stderr, "Failure seek end of file!\n");
        goto out0;
    }
    fileSize = ftell(file);
    rewind(file);
    startFile = (BYTE *)malloc(fileSize * sizeof(BYTE));
    if(startFile == NULL)
    {
        fprintf(stderr, "Failure malloc for file!\n");
        goto out0;
    }

    if (!fread(startFile, fileSize, 1, file))
    {
        fprintf(stderr, "Failure reading file!\n");
        goto out0;
    }
    if (fclose(file))
    {
        fprintf(stderr, "Failure closing file\n");
        goto out0;
    }


/* Имитация алгоритма карвинга журнала */
    hasHeader = checkHasHeader(startFile);
    hasFooter = checkHasFooter(startFile, fileSize, &startFooter);
    if (hasFooter && (startFooter != (startFile + fileSize - FOOTER_SIZE)))
        isFileWrapped = true;

    eventLog.hasHeader = hasHeader;
    eventLog.hasFooter = hasFooter;
    eventLog.header = initParsedHeader();
    eventLog.footer = initParsedFooter();
    status = initEmptyArray(&eventLog.recordsArray);
    if (status != StatusOK)
    {
        fprintf(stderr, "Fail initial event log structure\n");
        goto out1;
    }

    status = parseJournal(startFile, fileSize, hasHeader, hasFooter, isFileWrapped, &eventLog);
    if (status != StatusOK)
    {
        fprintf(stderr, "Fail parsing journal\n");
        goto out2;
    }

#ifndef NDEBUG
    printf("\n===RELEASE OUTPUT===\n");
#endif

    printf("=== BASE INFO ===\n");
    printf("Number of records:\t%zu\n", eventLog.recordsArray.recordsCount);
    printf("Has header:\t\t%s\n", eventLog.hasHeader == true ? "Yes" : "No");
    printf("Has footer:\t\t%s\n", eventLog.hasFooter == true ? "Yes" : "No");
    printf("----------------------------------------------------------\n");
    printf("Parameter\t\theader info (footer info)\n"
           "----------------------------------------------------------\n");
    printHeaderFooter(&eventLog);


    if(!baseMode)
    {
        printf("\n=== RECORDS INFO ===\n"
               "Parameter\t\tfile info (recovered info)\n"
               "---------------------------------------------------\n");
        printEventLogRecords(&eventLog);
    }

out2:
    freeArray(&eventLog.recordsArray);
out1:
    free(startFile);
out0:
    return status;
}
