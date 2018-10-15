#include "cli.h"

#define LOG_PREFIX "CLI"
#include "log.h"

#define LINE_MAX 128
#define CHAR_INVALID 0

#define CLI_ARGC_MAX 10

static char line_buf[LINE_MAX];
static char *line_ptr;

static struct cli_entry *entries;

static char *line_end_ptr()
{
    /* Keep an extra char for the '\0' */
    return line_buf + LINE_MAX - 1;
}

static void line_reset_silent(void)
{
    line_ptr = line_buf;
}

static void line_reset(void)
{
    line_reset_silent();
    Serial.print("> ");
}

static void spaces_skip(char **ptr, const char *end)
{
    for (; (*ptr < end) && isspace(**ptr); (*ptr)++);
}

static void non_spaces_skip(char **ptr, const char *end)
{
    for (; (*ptr < end) && !isspace(**ptr); (*ptr)++);
}

static int cli_help_handler(int argc, char ** const argv)
{
    struct cli_entry *e;

    cli_printf("Help:\n");

    for (e = entries; e != NULL; e = e->next)
    {
        cli_printf("%s\n", e->cmd);
        cli_printf("\t%s\n", e->help);
    }

    return 0;
}

static struct cli_entry help_cli = {
    .cmd = "help",
    .handler = cli_help_handler,
    .help = "This help command"
};

void cli_init(void)
{
    line_reset_silent();
    entries = NULL;

    cli_entry_add(&help_cli);

    log_printf("Ready\n");
}

void cli_loop(void)
{
    char c = CHAR_INVALID;
    bool line_ready = false;
    struct cli_entry *entry;

    /* Do Serial work */
    if (Serial.available())
    {
        c = Serial.read();

        /* Echo */
        if (c != '\b')
            Serial.print(c);
    }

    /* Check the char */
    switch (c)
    {
    case CHAR_INVALID:
        /* No new char */
        break;
    case '\r':
        /* Ignore */
        break;
    case '\n':
        *line_ptr = '\0';
        line_ready = true;
        break;
    case 0x03: /* Ctrl+C */
        Serial.println("^C");
        line_reset();
        break;
    case 0x04: /* Ctrl+D */
        Serial.println("^D");
        line_reset();
        break;
    case '\b': /* Backspace */
        if (line_ptr > line_buf)
        {
            line_ptr--;
            /* Delete the already printed char with space */
            Serial.print("\b \b");
        }
        break;
    default:
        if (isprint(c))
        {
            if (line_ptr < line_end_ptr())
            {
                *line_ptr = c;
                line_ptr++;
            }
            else
            {
                log_printf("Error: line too long, dropping.\n");
                line_reset();
            }
        }
    }

    /* Process ready line */
    if (line_ready)
    {
        int argc = 0;
        char *argv[CLI_ARGC_MAX] = {NULL};
        char *split_ptr = line_buf;

        /* Split into arguments */
        while ((split_ptr < line_ptr) && (argc < CLI_ARGC_MAX))
        {
            spaces_skip(&split_ptr, line_ptr);

            argv[argc] = split_ptr;
            non_spaces_skip(&split_ptr, line_ptr);
            *split_ptr = '\0';

            if (argv[argc] != split_ptr)
                argc++;
            
            split_ptr++;
        }

        for (int i = 0; i < argc; i++)
        {
            log_debug_printf("arg %d: '%s'\n", i, argv[i]);
        }

        /* Find the right command */
        if (argc > 0) {
            for (entry = entries; entry != NULL; entry = entry->next)
            {
                log_debug_printf("Entry: cmd = %s\n", entry->cmd);

                if (strcmp(entry->cmd, argv[0]) == 0) {
                    int ret;
                    /* Execute the right handler */
                    ret = entry->handler(argc, argv);

                    if (ret)
                        cli_printf("FAIL! ret = %d\n", ret);

                    break;
                }
            }

            if (entry == NULL) {
                cli_printf("Unknown Command!\n");
            }
        }

        line_reset();
        line_ready = false;
    }
}

void cli_entry_add(struct cli_entry *e)
{
    if (entries == NULL)
    {
        entries = e;
    }
    else
    {
        e->next = entries;
        entries = e;
    }
}
