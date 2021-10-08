#ifndef __MORSE_H
#define __MORSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <string.h>
#include <stdlib.h>

typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t  u8;
typedef int32_t  i32;
typedef int32_t  bool;

#define DOT_DURATION 200

typedef struct {
    bool led_on;
    u16  duration;
    // char source;
} MorseCommand;

const char *MORSE_ALPHABET[26] = {
    /* A */ "._",
    /* B */ "_...",
    /* C */ "_._.",
    /* D */ "_..",
    /* E */ ".",
    /* F */ ".._.",
    /* G */ "__.",
    /* H */ "....",
    /* I */ "..",
    /* J */ ".___",
    /* K */ "_._",
    /* L */ "._..",
    /* M */ "__",
    /* N */ "_.",
    /* O */ "___",
    /* P */ ".__.",
    /* Q */ "__._",
    /* R */ "._.",
    /* S */ "...",
    /* T */ "_",
    /* U */ ".._",
    /* V */ "..._",
    /* W */ ".__",
    /* X */ "_.._",
    /* Y */ "_.__",
    /* Z */ "__..",
};
const char *MORSE_DIGITS[10] = {
    /* 0 */ "_____",
    /* 1 */ ".____",
    /* 2 */ "..___",
    /* 3 */ "...__",
    /* 4 */ "...._",
    /* 5 */ ".....",
    /* 6 */ "_....",
    /* 7 */ "__...",
    /* 8 */ "___..",
    /* 9 */ "____.",
};
const char *MORSE_SPACE = "/";

/**
 * @brief Convert a character to its Morse code representation.
 * 
 * @param[in] c     The character to convert
 * @return A Morse code string, consisting of . and _, or / on word break. 
 */
const char *get_morse_char_v2(char c) {
    // numeric
    if (47 < c && c < 58) {
        return MORSE_DIGITS[c - 48];
    }
    // alphabetic
    if (64 < c && c < 91) {
        return MORSE_ALPHABET[c - 65];
    }
    // space
    if (c == 32) {
        return MORSE_SPACE;
    }
    // unknown
    return "";
}

/**
 * @brief Create a timing sequence that represents the `message` as Morse code.
 *        Allocates memory for `sequence`, which needs to be freed manually.
 * 
 * @param[in]  message      The message to convert.
 * @param[out] sequence     The array to write the sequence to.
 * @param[out] seq_length   The created sequence's length.
 */
void create_morse_sequence(const char *message, MorseCommand **sequence, u32 *seq_length) {
    u32 msg_length, seq_iter;

    *seq_length = 0;
    *sequence = NULL;

    // Reserve memory for characters
    msg_length = strlen(message);
    const char* morse_chars[msg_length];

    // Collect characters and calculate required memory
    for (int i = 0; i < msg_length; i++) {
        const char *subseq;

        subseq = get_morse_char_v2(message[i]);
        morse_chars[i] = subseq;
        *seq_length += strlen(subseq) * 2; // dot/dash count + gaps
    }
    *sequence = (MorseCommand *)malloc(*seq_length * sizeof(MorseCommand));

    // Step through each character and create timing sequence
    seq_iter = 0;
    for (int i = 0; i < msg_length; i++) {
        const char *subseq;
        u32 subseq_length;

        // Fetch morse character
        subseq = morse_chars[i];
        subseq_length = strlen(subseq);

        for (int j = 0; j < subseq_length; j++) {
            char c;
            MorseCommand *cmd, *gap;

            c = subseq[j];
            cmd = &(*sequence)[seq_iter++];
            gap = &(*sequence)[seq_iter++];

            // cmd->source = c;
            // gap->source = 'X';

            // Calculate gap duration
            gap->led_on = 0;
            if (j < subseq_length - 1) {
                // Dot/dash gap
                gap->duration = DOT_DURATION;
            }
            else {
                // Character-to-character gap
                gap->duration = DOT_DURATION * 2;
            }

            // Calculate character duration
            if (c == '.') {
                cmd->led_on = 1;
                cmd->duration = DOT_DURATION;
            } 
            else if (c == '_') {
                cmd->led_on = 1;
                cmd->duration = DOT_DURATION * 3;
            }
            else if (c == '/') {
                cmd->led_on = 0;
                cmd->duration = DOT_DURATION * 4;
            }
        }
    }
}

/** The message to transmit as Morse code */
const char *message = "AAAA BBBB";
/** The message as a timing sequence (array) */
MorseCommand *sequence = NULL;
/** Length of the timing sequence */
u32 sequence_length = 0;

/** The currently active command */
MorseCommand *current_command = NULL;
/** Index of the currently active command */
u32 sequence_idx = 0;
/** Timestamp (ms) indicating when the next command should begin */
u32 next_command_time = 0;

void HAL_SYSTICK_Callback(void) {
    // Load sequence if it wasn't initialized
    if (sequence == NULL) {
        create_morse_sequence(message, &sequence, &sequence_length);
        HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin, 0);
    }
    // Step through sequence
    if (HAL_GetTick() > next_command_time && sequence_idx < sequence_length) {
        current_command = &sequence[sequence_idx];
        sequence_idx++;
        next_command_time = HAL_GetTick() + current_command->duration;
        HAL_GPIO_WritePin(GPIOD, LD3_Pin, current_command->led_on);
    }
    // Turn on green LED to indicate success
    if (sequence_idx >= sequence_length) {
        free(sequence);
        HAL_GPIO_WritePin(GPIOD, LD3_Pin, 0);
        HAL_GPIO_WritePin(GPIOD, LD4_Pin, 1);
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __MORSE_H */
