#include "evt-common.h"

Status initArray(RecordsArray *arr, size_t initialCapacity)
{
    arr->records = malloc(initialCapacity * sizeof(ParsedRecord));
    if (arr == NULL)
        return StatusError;
    arr->recordsCount = 0;
    arr->capacity = initialCapacity;
    return StatusOK;
}

Status initEmptyArray(RecordsArray *arr)
{
    return initArray(arr, 0);
}

Status insertArray(RecordsArray *arr, ParsedRecord element)
{
    if (arr->recordsCount == arr->capacity)
    {
        (arr->capacity == 0) ? (arr->capacity = 1) : (arr->capacity *= 2);
        ParsedRecord* temp = realloc(arr->records, arr->capacity * sizeof(ParsedRecord));
        if (temp == NULL)
            return StatusError;
        arr->records = temp;
    }
    arr->records[arr->recordsCount++] = element;
    return StatusOK;
}

void freeArray(RecordsArray *arr)
{
    free(arr->records);
    arr->records = NULL;
    arr->recordsCount = arr->capacity = 0;
}

EventLogRecordHeader initEventLogRecordHeader()
{
    EventLogRecordHeader recordHeader;
    recordHeader.length = UNDEF_DWORD;
    recordHeader.signature = UNDEF_DWORD;
    recordHeader.recordNumber = UNDEF_DWORD;
    recordHeader.timeGenerated = UNDEF_DWORD;
    recordHeader.timeWritten = UNDEF_DWORD;
    recordHeader.eventID = UNDEF_DWORD;
    recordHeader.eventType = UNDEF_WORD;
    recordHeader.numStrings = UNDEF_WORD;
    recordHeader.eventCategory = UNDEF_WORD;
    recordHeader.reservedFlags = UNDEF_WORD;
    recordHeader.closingRecordNumber = UNDEF_DWORD;
    recordHeader.stringOffset = UNDEF_DWORD;
    recordHeader.userSidLength = UNDEF_DWORD;
    recordHeader.userSidOffset = UNDEF_DWORD;
    recordHeader.dataLength = UNDEF_DWORD;
    recordHeader.dataOffset = UNDEF_DWORD;

    recordHeader.recoveredLength = UNDEF_DWORD;
    recordHeader.recoveredDataLength = UNDEF_DWORD;
    return recordHeader;
}

ParsedRecord initParsedRecord()
{
    ParsedRecord record;
    record.recordHeader = initEventLogRecordHeader();
    record.sourceName = NULL;
    record.computerName = NULL;
    record.strings = NULL;
    record.data = NULL;
    record.isHidden = false;
    return record;
}

ParsedHeader initParsedHeader()
{
    ParsedHeader header;
    header.headerSize = UNDEF_DWORD;
    header.majorVersion = UNDEF_DWORD;
    header.minorVersion = UNDEF_DWORD;
    header.startOffset = UNDEF_DWORD;
    header.endOffset = UNDEF_DWORD;
    header.currentRecordNumber = UNDEF_DWORD;
    header.oldestRecordNumber = UNDEF_DWORD;
    header.fileSize = UNDEF_DWORD;
    header.flags = UNDEF_DWORD;
    header.retention = UNDEF_DWORD;
    header.endHeaderSize = UNDEF_DWORD;

    header.recoveredCurrentRecordNumber = UNDEF_DWORD;
    header.recoveredOldestRecordNumber = UNDEF_DWORD;
    header.recoveredFileSize = UNDEF_DWORD;
    return header;
}

ParsedFooter initParsedFooter()
{
    ParsedFooter footer;
    footer.size = UNDEF_DWORD;
    footer.one = UNDEF_DWORD;
    footer.two = UNDEF_DWORD;
    footer.three = UNDEF_DWORD;
    footer.four = UNDEF_DWORD;
    footer.beginRecord = UNDEF_DWORD;
    footer.endRecord = UNDEF_DWORD;
    footer.currentRecordNumber = UNDEF_DWORD;
    footer.oldestRecordNumber = UNDEF_DWORD;
    footer.recordSizeEnd = UNDEF_DWORD;
    return footer;
}

void printNumericElement(const char* nameElem, const DWORD elem)
{
    printf("%s", nameElem);
    if(elem == UNDEF_DWORD) { printf("unknown"); }
    else { printf("%d", elem); }
    printf("\n");
}

void printNumericElementWithRecovering(const char* nameElem,
                                       const DWORD elem,
                                       const DWORD elemRecover)
{
    printf("%s", nameElem);
    if(elem == UNDEF_DWORD) { printf("unknown"); }
    else { printf("%d", elem); }
    if (elemRecover == UNDEF_DWORD) { printf("\n"); }
    else { printf(" (%d)\n", elemRecover); }
}

void printTimeElement(const char* nameElem, const DWORD elem)
{
    time_t timer = elem;
    char* buff;
    buff = malloc(sizeof("hh:mm:ss UTC dd.MM.yyyy"));
    struct tm* tm_info = localtime(&timer);
    printf("%s", nameElem);
    if(!strftime(buff, sizeof("hh:mm:ss UTC dd.MM.yyyy"), "%T %Z %d.%m.%Y", tm_info) ||
        elem == UNDEF_DWORD) { printf("unknown"); }
    else { printf("%s\n", buff); }
    free(buff);
}

void printParsedRecord(const ParsedRecord* record)
{
    if (record->isHidden == true)
        printf("!-- Hidden record --!\n");

    printNumericElement("Event number\t\t", record->recordHeader.recordNumber);
    printTimeElement("Time written\t\t", record->recordHeader.timeWritten);
    printTimeElement("Time generated\t\t", record->recordHeader.timeGenerated);
    printNumericElementWithRecovering("Length (bytes)\t\t", record->recordHeader.length,
                                      record->recordHeader.recoveredLength);
    printNumericElement("Event ID\t\t", record->recordHeader.eventID);
    printNumericElement("Number of strings\t", record->recordHeader.numStrings);
    printNumericElementWithRecovering("Data length (bytes)\t", record->recordHeader.dataLength,
                                      record->recordHeader.recoveredDataLength);
}

void printHeaderFooter(const EventLog* eventLog)
{
    printNumericElementWithRecovering("Current record number\t", eventLog->header.currentRecordNumber,
                                      eventLog->footer.currentRecordNumber);
    printNumericElementWithRecovering("Oldest record number\t", eventLog->header.oldestRecordNumber,
                                      eventLog->footer.oldestRecordNumber);
    printNumericElement("File size (bytes)\t", eventLog->header.fileSize);
}

void printEventLogRecords(const EventLog* eventLog)
{
    size_t ind;
    for (ind = 0; ind < eventLog->recordsArray.recordsCount; ind++)
    {
        printParsedRecord(&eventLog->recordsArray.records[ind]);
        printf("\n");
    }
}
