#include "evt-functions.h"

CBYTE SIGNATURE_RECORD[] = { 0x4c, 0x66, 0x4c, 0x65 };
const size_t SIGNATURE_RECORD_SIZE = sizeof(SIGNATURE_RECORD);

CBYTE SIGNATURE_HEADER_START[] = { 0x30, 0x00, 0x00, 0x00, 0x4c, 0x66, 0x4c, 0x65 };
CBYTE SIGNATURE_HEADER_END[] = { 0x30, 0x00, 0x00, 0x00 };

const size_t HEADER_SIZE = 0x30;
const size_t FOOTER_SIZE = 0x28;

CBYTE SIGNATURE_FOOTER_START[] = {0x28, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44};
CBYTE SIGNATURE_FOOTER_END[] = {0x28, 0x00, 0x00, 0x00};

const size_t PADDING_SIZE = 8;

/* =========================================================*/
/* ================SEARCHING SIGNATURE======================*/
/* =========================================================*/
CBYTE* searchSignature(CBYTE* buffer, size_t bufferSize,
                       CBYTE* signature, size_t signatureSize)
{
    assert(buffer != NULL);
    assert(signature != NULL);

    const BYTE* pointer;

    if (bufferSize == 0)
        return NULL;
    if (signatureSize == 0)
        return NULL;

    for (pointer = buffer; bufferSize >= signatureSize;
        ++pointer, --bufferSize)
    {
        if (memcmp(pointer, signature, signatureSize) == 0)
        {
            return pointer;
        }
    }
    return NULL;
}

CBYTE* searchSignatureWithinBoundaries(CBYTE* start, CBYTE* end, CBYTE* signature, size_t signatureSize)
{
    if (start > end)
    {
#ifndef NDEBUG
        printf("searchSignature: start of area is bigger than end.\n");
#endif
        return NULL;
    }
    return searchSignature(start, end - start, signature, signatureSize);
}

/* =========================================================*/
/* ====================PARSING RECORD=======================*/
/* =========================================================*/

Status parseDWordElem(DWORD* elem, CBYTE** cur, CBYTE* end)
{
    if (*cur + sizeof(*elem) > end)
        return StatusStop;
    memcpy(elem, *cur, sizeof(*elem));
    *cur += sizeof(*elem);
    return StatusOK;
}

Status parseWordElem(WORD* elem, CBYTE** cur, CBYTE* end)
{
    if (*cur + sizeof(*elem) > end)
        return StatusStop;
    memcpy(elem, *cur, sizeof(*elem));
    *cur += sizeof(*elem);
    return StatusOK;
}

Status parseHeader(CBYTE* startHeader, ParsedHeader* header)
{
    CBYTE* cur = startHeader;
    CBYTE* endHeader = startHeader + HEADER_SIZE;
    if (parseDWordElem(&header->headerSize, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->signature, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->majorVersion, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->minorVersion, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->startOffset, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->endOffset, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->currentRecordNumber, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->oldestRecordNumber, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->fileSize, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->flags, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->retention, &cur, endHeader) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&header->endHeaderSize, &cur, endHeader) == StatusStop)
        return StatusOK;

#ifndef NDEBUG
    printf("--Start Parsing header--\n");
    printf("size\t\t\t0x%08X\n", header->headerSize);
    printf("currentRecordNumber\t0x%08X\n", header->currentRecordNumber);
    printf("--STOP Parsing header--\n");
#endif
    return StatusOK;
}

Status parseFooter(CBYTE* startFooter, ParsedFooter* footer)
{
    CBYTE* cur = startFooter;
    CBYTE* endFooter = startFooter + FOOTER_SIZE;
    if (parseDWordElem(&footer->size, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->one, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->two, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->three, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->four, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->beginRecord, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->endRecord, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->currentRecordNumber, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->oldestRecordNumber, &cur, endFooter) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&footer->recordSizeEnd, &cur, endFooter) == StatusStop)
        return StatusOK;

#ifndef NDEBUG
    printf("--Start Parsing footer--\n");
    printf("size\t\t 0x%08X\n", footer->size);
    printf("currentRecordNumber\t 0x%08X\n", footer->currentRecordNumber);
    printf("--STOP Parsing footer--\n");
#endif
    return StatusOK;
}

Status parseRecord(CBYTE* startRecord, size_t recordSize, ParsedRecord* record)
{
    CBYTE* cur = startRecord;
    CBYTE* endRecord = startRecord + recordSize;
    if (parseDWordElem(&record->recordHeader.length, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.signature, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.recordNumber, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.timeGenerated, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.timeWritten, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.eventID, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseWordElem(&record->recordHeader.eventType, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseWordElem(&record->recordHeader.numStrings, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseWordElem(&record->recordHeader.eventCategory, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseWordElem(&record->recordHeader.reservedFlags, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.closingRecordNumber, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.stringOffset, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.userSidLength, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.userSidOffset, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.dataLength, &cur, endRecord) == StatusStop)
        return StatusOK;
    if (parseDWordElem(&record->recordHeader.dataOffset, &cur, endRecord) == StatusStop)
        return StatusOK;

    record->data = (BYTE*)startRecord + record->recordHeader.dataOffset;

#ifndef NDEBUG
    printf("--Start Parsing record--\n");
    printf("length\t\t 0x%08X\n", record->recordHeader.length);
    printf("recordNumber\t 0x%08X\n", record->recordHeader.recordNumber);
    printf("eventCategory\t 0x%04X.\n", record->recordHeader.eventCategory);
    printf("dataOffset\t 0x%08X.\n", record->recordHeader.dataOffset);
    printf("dataLength\t 0x%08X\n", record->recordHeader.dataLength);
    printf("--STOP Parsing record--\n");
#endif
    return StatusOK;
}


/* =========================================================*/
/* ============CHECKING HEADER AND FOOTER===================*/
/* =========================================================*/
bool checkHasHeader(CBYTE* buffer)
{
    bool hasStartSignature = (searchSignature(buffer, sizeof(SIGNATURE_HEADER_START), SIGNATURE_HEADER_START, sizeof(SIGNATURE_HEADER_START)) != NULL);
    bool hasEndSignature = (searchSignature(buffer, sizeof(SIGNATURE_HEADER_END), SIGNATURE_HEADER_END, sizeof(SIGNATURE_HEADER_END)) != NULL);
    return (hasStartSignature && hasEndSignature);
}

bool checkHasFooter(CBYTE* buffer, size_t bufferSize, BYTE** startFooter)
{
    *startFooter = (BYTE*)searchSignature(buffer, bufferSize, SIGNATURE_FOOTER_START, sizeof(SIGNATURE_FOOTER_START));
    if (*startFooter != NULL)
        return (searchSignature(*startFooter, bufferSize - (*startFooter - buffer), SIGNATURE_FOOTER_END, sizeof(SIGNATURE_FOOTER_END)) != NULL);
    return false;
}


/* =========================================================*/
/* =============RESTORE HIDDEN RECORDS======================*/
/* =========================================================*/
Status restoreHiddenRecords(ParsedRecord* record, RecordsArray* allRecords)
{
#ifndef NDEBUG
    printf("Search of hidden records is started.\n");
#endif
    Status status;

    status = insertArray(allRecords, *record);
    if (status != StatusOK)
        return status;

    /* указатель на обрабатываемую запись для удобства */
    size_t origIndex = allRecords->recordsCount - 1;
    ParsedRecord* origRecord = &allRecords->records[origIndex];

    /* Указатель на первую найденную скрытую запись для проверки  */
    BYTE* firstHiddenRecord = origRecord->data;

    /* Указатель на сигнатуру, размер и начало скрытой записи */
    BYTE* curSign = NULL;
    size_t size = 0;
    BYTE* begin = NULL;
    BYTE* endData = origRecord->data + origRecord->recordHeader.dataLength;;
    size_t checkSize = 0;

    /* Указатель на начало обрабатываемой в цикле области.
     * начало с отступом в два поля: оригинальные padding и length */
    BYTE* curStart = origRecord->data + PADDING_SIZE;

    size_t origDataSize = 0;

start:
    curSign = (BYTE*)searchSignatureWithinBoundaries(curStart, endData, SIGNATURE_RECORD, SIGNATURE_RECORD_SIZE);
    if (curSign == NULL)
    {
#ifndef NDEBUG
        printf("End of searching hidden records.\n");
#endif
        return StatusStop;
    }

    begin = curSign - 4;
    memcpy(&size, begin, 4);

    /*  При скрытии у последней скрытой записи будет на 8 байт меньше,
     *  т.к. они будут считаться за padding и Length основной записи и
     *  будут уже вне поля data */
    if (begin + size <= endData + PADDING_SIZE)
    {
        memcpy(&checkSize, (begin + size - 4), 4);

#ifndef NDEBUG
        printf("check size %s size\n", checkSize == size ? "=" : "!=");
        printf("check size %s origSize\n", checkSize == (size_t)origRecord->recordHeader.length ? "=" : "!=");
#endif

        if ((checkSize == size) || (checkSize == (size_t)origRecord->recordHeader.length))
        {
            curStart = begin + size;
            memcpy(&checkSize, curStart, 4);
            /* если записей еще не находили, то выставляем указатель на начало найденной */
            if (firstHiddenRecord == origRecord->data)
                firstHiddenRecord = begin;

            ParsedRecord curPRecord = initParsedRecord();
            status = parseRecord(begin, size, &curPRecord);
            if (status != StatusOK)
                return status;

            curPRecord.isHidden = true;
            status = insertArray(allRecords, curPRecord);
            origRecord = &allRecords->records[origIndex];
            if (status != StatusOK)
                return status;

            if (curStart <= endData )
            {
                goto start;
            }
            else
            {
                goto checkHasOwnData;
            }
        }
        else
        {

#ifndef NDEBUG
    printf("Size is different! 0x%04X != 0x%04X. I'm going to search next record.\n", (unsigned int)size, (unsigned int)checkSize);
#endif
            curStart += PADDING_SIZE;
            goto start;
        }
    }


checkHasOwnData:
    /* если первую запись нашли не в начале даты,
     * значит, в ней что-то лежало до скрытия */
    if (firstHiddenRecord != origRecord->data + PADDING_SIZE)
        origDataSize = firstHiddenRecord - origRecord->data - PADDING_SIZE;

    origRecord->recordHeader.recoveredDataLength = (DWORD)origDataSize;
    origRecord->recordHeader.recoveredLength = origRecord->recordHeader.length - origRecord->recordHeader.dataLength + (DWORD)origDataSize;
#ifndef NDEBUG
    printf("origDataSize = %zu\n", origDataSize);
    printf("restored legth is 0x%04X\n", origRecord->recordHeader.recoveredLength);
    printf("End of searching of hidden records.\n");
#endif
    return StatusOK;
}

/* =========================================================*/
/* =====================CARVING RECORD======================*/
/* =========================================================*/
Status carveRecord(CBYTE* startRecords, CBYTE* endRecords,
                   CBYTE** curStart, BYTE** record,
                   size_t* recordSize, bool* isWrap)
{
    BYTE* curSignature = NULL;
    BYTE* begin = NULL;
    size_t checkSize = 0;
    BYTE* end = NULL;

start:
    curSignature = (BYTE*) searchSignatureWithinBoundaries(*curStart, endRecords, SIGNATURE_RECORD, SIGNATURE_RECORD_SIZE);

    if (curSignature == NULL)
    {
        printf("Carving record: Signature not found. Search is over.\n");
        return StatusStop;
    }

    begin = (curSignature - SIGNATURE_RECORD_SIZE);
    memcpy(recordSize, begin, SIGNATURE_RECORD_SIZE);

    end = (begin + *recordSize);
    if ( end <= endRecords)
    {
        memcpy(&checkSize, (end - 4), 4);

        if (checkSize != *recordSize)
        {

#ifndef NDEBUG
    printf("Size is different! 0x%04X != 0x%04X. I'm going to search next record.\n", (unsigned int)*recordSize, (unsigned int)checkSize);
#endif
            *curStart += PADDING_SIZE;
            goto start;
        }
        else
        {
            *curStart = begin + *recordSize;
            *record = begin;
            return StatusOK;
        }
    }

    /* обработка закольцованной структуры */
#ifndef NDEBUG
    printf("Record is out of boundaries.\n");
#endif
    memcpy(&checkSize, (startRecords + (*recordSize - (endRecords - begin))) - 4, 4);
    if (checkSize != *recordSize)
    {
#ifndef NDEBUG
    printf("Size is different! 0x%04X != 0x%04X. I'm going to search next record.\n", (unsigned int)*recordSize, (unsigned int)checkSize);
#endif
        *curStart = (BYTE*)endRecords;
        *recordSize = endRecords - begin;
        *record = begin;
        return StatusOK;
    }
    *isWrap = true;
    *record = begin;

    return StatusOK;
}


/* =========================================================*/
/* =====================PARSING JOURNAL=====================*/
/* =========================================================*/
Status parseJournal(CBYTE* startEventLog, size_t eventLogSize,
                    bool hasHeader, bool hasFooter,
                    bool isFileWrapped, EventLog* eventLog)
{
#ifndef NDEBUG
    printf("Parsing journal...\n");
#endif
    CBYTE* startRecords = NULL;
    CBYTE* endRecords = NULL;
    CBYTE* startFooter = NULL;
    CBYTE* curStart = NULL;
    BYTE* curRecord = NULL;
    size_t curRecordSize = 0;
    bool isWrap = false;
    Status status = StatusOK;

    startRecords = startEventLog;

    if (hasHeader)
        startRecords += HEADER_SIZE;

    curStart = (BYTE*)startRecords;

    if (isFileWrapped)
    {
        endRecords = startEventLog + eventLogSize;
        startFooter = searchSignature(startRecords, endRecords - startRecords, SIGNATURE_FOOTER_START, sizeof(SIGNATURE_FOOTER_START));
    }
    else
    {
        if (hasFooter)
        {
            endRecords = startEventLog + eventLogSize - FOOTER_SIZE;
            startFooter = endRecords;
        }
        else
        {
            endRecords = startEventLog + eventLogSize;
        }

    }

    while (isWrap != true && curStart < endRecords)
    {
        status = carveRecord(startRecords, endRecords, &curStart, &curRecord, &curRecordSize, &isWrap);
        if(status != StatusOK)
            return status;
        if (!isWrap)
        {
            ParsedRecord curPRecord = initParsedRecord();
            status = parseRecord(curRecord, curRecordSize, &curPRecord);
            if (status != StatusOK)
                return status;

            restoreHiddenRecords(&curPRecord, &eventLog->recordsArray);
        }
        else /* обработка закольцованной записи */
        {
            BYTE* wrappedRecord = malloc(curRecordSize);
            if (wrappedRecord == NULL)
                return StatusError;
            size_t sizeTillEnd = endRecords - curRecord;
            memcpy(wrappedRecord, curRecord, sizeTillEnd);
            memcpy(wrappedRecord + sizeTillEnd, startRecords, curRecordSize - sizeTillEnd);
            ParsedRecord curPRecord = initParsedRecord();
            status = parseRecord(wrappedRecord, curRecordSize, &curPRecord);
            restoreHiddenRecords(&curPRecord, &eventLog->recordsArray);
            free(wrappedRecord);
            if (status != StatusOK)
                return status;
        }
    }


    if(hasHeader)
    {
        status = parseHeader(startEventLog, &eventLog->header);
        if (status != StatusOK)
            return status;
    }

    if(hasFooter)
    {
        status = parseFooter(startFooter, &eventLog->footer);
        if (status != StatusOK)
            return status;
    }

    return status;
}
