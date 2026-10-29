#if LV_BUILD_TEST
#include "../lvgl.h"

#include "unity/unity.h"

void setUp(void)
{
    /* Function run before every test */
}

void tearDown(void)
{
    /* Function run after every test */
    lv_obj_clean(lv_screen_active());
}

/* A component that parses but has no <view> must be rejected without leaving
 * anything behind in the component list (the next lookup used to read a freed node). */
void test_xml_component_without_view_is_rejected(void)
{
    const char * no_view_xml =
        "<component>"
        "<consts><const name=\"c\" value=\"1\"/></consts>"
        "</component>";

    lv_result_t res = lv_xml_component_register_from_data("no_view", no_view_xml);
    TEST_ASSERT_EQUAL(LV_RESULT_INVALID, res);

    TEST_ASSERT_NULL(lv_xml_component_get_scope("no_view"));
    TEST_ASSERT_EQUAL(LV_RESULT_INVALID, lv_xml_component_unregister("no_view"));

    /* The component list must still be usable afterwards. */
    const char * ok_xml =
        "<component>"
        "<view extends=\"lv_label\" text=\"ok\"/>"
        "</component>";

    TEST_ASSERT_EQUAL(LV_RESULT_OK, lv_xml_component_register_from_data("ok_comp", ok_xml));
    TEST_ASSERT_NOT_NULL(lv_xml_component_get_scope("ok_comp"));

    lv_obj_t * obj = lv_xml_create(lv_screen_active(), "ok_comp", NULL);
    TEST_ASSERT_NOT_NULL(obj);

    TEST_ASSERT_EQUAL(LV_RESULT_OK, lv_xml_component_unregister("ok_comp"));
    TEST_ASSERT_NULL(lv_xml_component_get_scope("ok_comp"));
}

/* Malformed XML must be rejected and must not register anything. */
void test_xml_component_malformed_is_rejected(void)
{
    const char * truncated_xml =
        "<component>"
        "<consts><const name=\"c\" value=\"1\"/></consts>"
        "<view";

    lv_result_t res = lv_xml_component_register_from_data("truncated", truncated_xml);
    TEST_ASSERT_EQUAL(LV_RESULT_INVALID, res);
    TEST_ASSERT_NULL(lv_xml_component_get_scope("truncated"));
}

#endif
