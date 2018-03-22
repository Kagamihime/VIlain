#include <stdlib.h>
#include <check.h>
#include "../include/buffer.h"
#include "../include/cursor.h"
#include "../include/io_text.h"
#include "../include/settings.h"
#include "../include/ui.h"

int main(void)
{
    int failures;
    Suite *s;
    SRunner *sr;

    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failures = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
