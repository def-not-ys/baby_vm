/* private helpers for memory */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "types_private.h"

extern uint16_t _memory[];
extern Hashmap hashmap;

static void _examine_memory()
{
    // examine _memory[]
    printf("\n\n");
    for (int i = 0; i < UINT16_MAX; i++ )
    {
        if (_memory[i] != 0)
        {
            printf("_memory[0x%x] = 0x%x\n", i, _memory[i]);
        }
    }
}

/*
 * check if a token is label (ends with ':')
 */
static bool _is_label(char* token)
{
    assert(NULL != token && "invalid null token");
    int len = strlen(token);
    if (':' == token[len-1])
    {
        return TRUE;
    }
    return FALSE;
}

/*
 * retrive token value in text and return the int value
 */
static uint16_t _get_token_value(char* token)
{
    // @TODO: retrive token value in text and return the int value
    return 99;
}

/*
 * processes labels and saves { label: addr } in hashmap
 */

static HashmapStatus _process_label(char* token, int pos, int lower_bound, int upper_bound)
{
    HashmapStatus _status = STATUS_OK;
    char* str = strtok(token, ":");
    assert(NULL != str && "invalid label");
    assert(pos >= lower_bound && pos < upper_bound && "region overflow");
    char* label = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    label = strcpy(label, str);
    _status = hashmap.insert(&hashmap, label, pos);
    return _status;
}

/*
 * handles token in sections accordingly
 */
static ErrorStatus _token_handler(char* token, Section* section)
{
    ErrorStatus status = ERR_NONE;
    HashmapStatus _status = STATUS_OK;
    static int is_arg = 0;

    printf("handling token [ %s ]\n", token);

    switch (*section)
    {
        case UINITIALIZED:
            if (strcmp(token, "section") == 0)
            {
                *section = SECTION;
            }
            else
            {
                LOG("no section identifier");
                status = ERR_ATTN;
            }
            break;
        case SECTION:
            if (strcmp(token, ".text") == 0)
            {
                *section = TEXT;
                is_arg = 0;
            }
            else if (strcmp(token, ".data") == 0)
            {
                *section = DATA;
                is_arg = 0;
            }
            else
            {
                printf("invalid section identifier [ %s ]\n", token);
                assert(0);
                status = ERR_ATTN;
            }
            break;
        case TEXT:
            if (strcmp(token, "subleq") == 0)
            {
                is_arg++;
            }
            else if (is_arg % 4 != 0)
            {
                uint16_t value = _get_token_value(token);
                assert(text_pos >= TEXT_REGION_START && text_pos < TEXT_REGION_LAST && "text region overflow");
                _memory[text_pos] = value;
                text_pos++;
                is_arg++;
            }
            else if (_is_label(token))
            {
                _status = _process_label(token, text_pos, TEXT_REGION_START, TEXT_REGION_LAST);
            }
            else if (strcmp(token, "section") == 0)
            {
                *section = SECTION;
            }
            else
            {
                printf("invalid text value [ %s ]\n", token);
                assert(0);
                status = ERR_ATTN;
            }
            break;
        case DATA:
            if (_is_label(token))
            {
                _status = _process_label(token, data_pos, DATA_REGION_START, DATA_REGION_LAST);
            }
            else if (strcmp(token, "section") == 0)
            {
                *section = SECTION;
            }
            else
            {
                assert(data_pos >= DATA_REGION_START && data_pos < DATA_REGION_LAST && "text region overflow");
                uint16_t value = _get_token_value(token);
                _memory[data_pos] = value;
                data_pos++;
            }
            break;
        default:
            assert(0 && "invalid section");
    }

    if (STATUS_OK != _status)
    {
        status = ERR_ATTN;
    }

    return status;
}
