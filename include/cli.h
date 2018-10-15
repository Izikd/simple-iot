#ifndef __CLI_H__
#define __CLI_H__

typedef int (*cli_handler_t)(int argc, char** const argv);

struct cli_entry {
    /* Command name */
    const char *cmd;
    /* Command handler */
    cli_handler_t handler;
    /* Help string */
    const char *help;

    /* Internal - next entry */
    struct cli_entry *next;
};

/* Initialize CLI platfrom */
void cli_init(void);
/* CLI platform loop */
void cli_loop(void);

/* 
 * Add a new entry to the CLI list of commands.
 * Entry should be preserved by the user, 
 * as it's not copied by this function.
 */
void cli_entry_add(struct cli_entry *entry);

/**
 * Print from within CLI handler implementation
 */
#define cli_printf(fmt, ...)     \
    Serial.printf(fmt, ##__VA_ARGS__)

#endif // __CLI_H__
