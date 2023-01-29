#ifndef EVT_FUNCTIONS_H
#define EVT_FUNCTIONS_H

#include "evt-common.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Функция поиска сигнатуры
 * @param[in] buffer -- область поиска сигнатуры
 * @param[in] bufferSize -- размер области поиска
 * @param[in] signature -- указатель на искомую сигнатуру
 * @param[in] signatureSize -- размер сигнатуры
 * @return указатель на найденную сигнатуру или NULL, если сигнатура не найдена
 */
CBYTE* searchSignature(CBYTE* buffer, size_t bufferSize,
                       CBYTE* signature, size_t signatureSize);

/**
 * @brief Функция поиска сигнатуры
 * @param[in] start -- указатель на начало области поиска
 * @param[in] end -- указатель на конец области поиска
 * @param[in] signature -- указатель на искомую сигнатуру
 * @param[in] signatureSize -- размер сигнатуры
 * @return указатель на найденную сигнатуру или NULL, если сигнатура не найдена
 */
CBYTE* searchSignatureWithinBoundaries(CBYTE* start, CBYTE* end,
                                       CBYTE* signature, size_t signatureSize);

/**
 * @brief Функция проверки наличия сигнатуры по указанному смещению
 * @param[in] buffer -- область поиска сигнатуры
 * @param[in] signature -- указатель на искомую сигнатуру
 * @param[in] signatureSize -- размер сигнатуры
 * @param[in] offset -- смещение от buffer
 * @return
 */
bool checkSignatureWithOffset(CBYTE* buffer, CBYTE* signature,
                              size_t signatureSize, size_t offset);

/**
 * @brief Утилитарная функция для проверки наличия заголовка у журнала по слабым критериям
 * @param[in] buffer -- указатель на начало области поиска
 * @return bool -- true, если заголовк есть, в противном случае false
 */
bool checkHasHeader(CBYTE* buffer);

/**
 * @brief Утилитарная функция для проверки наличия терминальной структуры у журнала по слабым критериям и установления ее начала
 * @param[in] buffer -- указатель на начало области поиска
 * @param[in] bufferSize -- размер области поиска
 * @param[out] startFooter -- указатель на начало терминальной структуры. NULL, если не найдена
 * @return
 */
bool checkHasFooter(CBYTE* buffer, size_t bufferSize, BYTE** startFooter);

/**
 * @brief Функция обработки журнала событий
 * @param[in] startEventLog --
 * @param[in] eventLogSize
 * @param[in] hasHeader
 * @param[in] hasFooter
 * @param[in] isFileWrapped
 * @param[out] eventLog
 * @return
 */
Status parseJournal(CBYTE* startEventLog, size_t eventLogSize,
                    bool hasHeader, bool hasFooter,
                    bool isFileWrapped, EventLog* eventLog);

Status parseRecord(CBYTE* startRecord, size_t recordSize, ParsedRecord* record);

Status parseHeader(CBYTE* startHeader, ParsedHeader* header);
Status parseFooter(CBYTE* startFooter, ParsedFooter* footer);

#endif /* EVT_FUNCTIONS_H */
