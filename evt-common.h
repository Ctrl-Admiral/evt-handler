#ifndef EVT_COMMON_H
#define EVT_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BYTE unsigned char
#define CBYTE const unsigned char
#define WORD int16_t
#define DWORD int32_t
#define WCHAR wchar_t


extern CBYTE SIGNATURE_RECORD[];
extern const size_t SIGNATURE_RECORD_SIZE;

extern CBYTE SIGNATURE_HEADER_START[];
extern CBYTE SIGNATURE_HEADER_END[];

extern CBYTE SIGNATURE_FOOTER_START[];
extern CBYTE SIGNATURE_FOOTER_END[];

extern const size_t HEADER_SIZE;
extern const size_t FOOTER_SIZE;

extern const size_t PADDING_SIZE;


typedef enum
{
    StatusOK = 0,
    StatusError = 1,
    StatusCritical = 2,
    StatusStop = 3,
} Status;


/**
 * @brief структура /logRecord.
 */
typedef struct
{
    DWORD length;
    DWORD signature;
    DWORD recordNumber;
    DWORD timeGenerated;
    DWORD timeWritten;
    DWORD eventID;
    WORD  eventType;
    WORD  numStrings;
    WORD  eventCategory;
    WORD  reservedFlags;
    DWORD closingRecordNumber;
    DWORD stringOffset;
    DWORD userSidLength;
    DWORD userSidOffset;
    DWORD dataLength;
    DWORD dataOffset;

    DWORD recoveredLength;
    DWORD recoveredDataLength;
} EventLogRecordHeader;

/**
 * @brief Структура записи события
 */
typedef struct
{
    EventLogRecordHeader recordHeader;

    WCHAR* sourceName;
    WCHAR* computerName;
    WCHAR* strings;
    BYTE*  data;

    bool isHidden;
} ParsedRecord;

ParsedRecord initParsedRecord();
void printParsedRecord(const ParsedRecord* record);


/**
 * @brief Структура заголовка журнала. Содержит только элементы с данными
 */
typedef struct
{
    DWORD headerSize;
    DWORD signature;
    DWORD majorVersion;
    DWORD minorVersion;
    DWORD startOffset;
    DWORD endOffset;
    DWORD currentRecordNumber;
    DWORD oldestRecordNumber;
    DWORD fileSize;
    DWORD flags;
    DWORD retention;
    DWORD endHeaderSize;

    DWORD recoveredCurrentRecordNumber;
    DWORD recoveredOldestRecordNumber;
    DWORD recoveredFileSize;
} ParsedHeader;

ParsedHeader initParsedHeader();

/**
 * @brief терминальная структура журнала. Содержит только элементы с данными
 */
typedef struct
{
    DWORD size;
    DWORD one;
    DWORD two;
    DWORD three;
    DWORD four;
    DWORD beginRecord;
    DWORD endRecord;
    DWORD currentRecordNumber;
    DWORD oldestRecordNumber;
    DWORD recordSizeEnd;

    DWORD recoveredCurrentRecordNumber;
    DWORD recoveredOldestRecordNumber;
} ParsedFooter, *PParsedFooter;

ParsedFooter initParsedFooter();

typedef struct
{
  ParsedRecord* records;
  size_t recordsCount;
  size_t capacity;
} RecordsArray;

Status initArray(RecordsArray *arr, size_t initialCapacity);

Status initEmptyArray(RecordsArray *arr);

Status insertArray(RecordsArray *arr, ParsedRecord element);

void freeArray(RecordsArray *arr);

typedef struct
{
    bool hasHeader;
    bool hasFooter;
    ParsedHeader header;
    RecordsArray recordsArray;
    ParsedFooter footer;
} EventLog;

void printEventLogRecords(const EventLog* eventLog);

void printHeaderFooter(const EventLog* eventLog);

#endif /* EVT_COMMON_H */
