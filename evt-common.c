#include "evt-common.h"

Status initArray(RecordsArray *arr, size_t initialCapacity)
{
  arr->records = malloc(initialCapacity * sizeof(ParsedRecord));
  if (arr == NULL)
    return StatusCritical;
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
      return StatusCritical; /* do not forget to free the original pointer in the caller */
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
    recordHeader.length = -1;
    recordHeader.signature = -1;
    recordHeader.recordNumber = -1;
    recordHeader.timeGenerated = -1;
    recordHeader.timeWritten = -1;
    recordHeader.eventID = -1;
    recordHeader.eventType = -1;
    recordHeader.numStrings = -1;
    recordHeader.eventCategory = -1;
    recordHeader.reservedFlags = -1;
    recordHeader.closingRecordNumber = -1;
    recordHeader.stringOffset = -1;
    recordHeader.userSidLength = -1;
    recordHeader.userSidOffset = -1;
    recordHeader.dataLength = -1;
    recordHeader.dataOffset = -1;

    recordHeader.recoveredLength = -1;
    recordHeader.recoveredDataLength = -1;
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
    header.headerSize = -1;
    header.majorVersion = -1;
    header.minorVersion = -1;
    header.startOffset = -1;
    header.endOffset = -1;
    header.currentRecordNumber = -1;
    header.oldestRecordNumber = -1;
    header.fileSize = -1;
    header.flags = -1;
    header.retention = -1;
    header.endHeaderSize = -1;

    header.recoveredCurrentRecordNumber = -1;
    header.recoveredOldestRecordNumber = -1;
    header.recoveredFileSize = -1;
    return header;
}

ParsedFooter initParsedFooter()
{
    ParsedFooter footer;
    footer.size = -1;
    footer.one = -1;
    footer.two = -1;
    footer.three = -1;
    footer.four = -1;
    footer.beginRecord = -1;
    footer.endRecord = -1;
    footer.currentRecordNumber = -1;
    footer.oldestRecordNumber = -1;
    footer.recordSizeEnd = -1;
    return footer;
}

void printNumericElement(const char* nameElem, const DWORD elem)
{
    printf("%s", nameElem);
    if(elem == -1) { printf("unknown"); }
    else { printf("%d", elem); }
    printf("\n");
}

void printNumericElementWithRecovering(const char* nameElem,
                                       const DWORD elem,
                                       const DWORD elemRecover)
{
    printf("%s", nameElem);
    if(elem == -1) { printf("unknown"); }
    else { printf("%d", elem); }
    if (elemRecover == -1) { printf("\n"); }
    else { printf(" (%d)\n", elemRecover); }
}

void printTimeElement(const char* nameElem, const DWORD elem)
{
    time_t timer = elem;
    char buff[30];
    struct tm* tm_info = localtime(&timer);
    printf("%s", nameElem);
    if(!strftime(buff, 30, "%T %Z %d.%m.%Y", tm_info) || elem == -1) { printf("unknown"); }
    else { printf("%s\n", buff); }
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
