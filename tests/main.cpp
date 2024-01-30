#include <iostream>
#include <iomanip>
#include <cstring>

#define TERMINAL_RED        "\033[91m"
#define TERMINAL_DEFAULT    "\033[39m"

#define ADD_TEST(test_name)                                     \
    do {                                                        \
        extern int test_name();                                 \
        if (argc >= 2 && !std::strcmp(argv[1], #test_name)) {   \
            std::cout << TERMINAL_RED;                          \
            int ret = test_name();                              \
            std::cout << TERMINAL_DEFAULT;                      \
            return ret;                                         \
        }                                                       \
    } while (false)


int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " test_name" << std::endl;
        return -1;
    }

    std::cout << std::setprecision(20);

    ADD_TEST(test_geometry_segment_point_dist_close_point);
    ADD_TEST(test_geometry_segment_point_dist_far_point);
    ADD_TEST(test_geometry_segment_point_dist_point_on_segment);
    ADD_TEST(test_geometry_segment_point_dist_zero_len_segment);

    ADD_TEST(test_geolib_segment_intersection_1);
    ADD_TEST(test_geolib_segment_intersection_2);
    ADD_TEST(test_geolib_segment_intersection_3);
    ADD_TEST(test_geolib_segment_intersection_4);
    ADD_TEST(test_geolib_segment_intersection_5);
    ADD_TEST(test_geolib_segment_intersection_6);
    ADD_TEST(test_geolib_segment_intersection_7);
    ADD_TEST(test_geolib_segment_intersection_8);
    ADD_TEST(test_geolib_segment_intersection_9);
    ADD_TEST(test_geolib_segment_intersection_10);
    ADD_TEST(test_geolib_segment_intersection_11);
    ADD_TEST(test_geolib_segment_intersection_12);

    ADD_TEST(test_geolib_read_wkt_point);
    ADD_TEST(test_geolib_read_wkt_segment);
    ADD_TEST(test_geolib_read_wkt_ring);
    ADD_TEST(test_geolib_read_wkt_polygon);
    ADD_TEST(test_geolib_read_wkt_multipolygon);

    ADD_TEST(test_geolib_convex_hull_1);

    ADD_TEST(test_geolib_point_index_1);
    ADD_TEST(test_geolib_point_index_2);
    ADD_TEST(test_geolib_point_index_3);
    ADD_TEST(test_geolib_point_index_4);
    ADD_TEST(test_geolib_point_index_5);
    ADD_TEST(test_geolib_point_index_6);
    ADD_TEST(test_geolib_point_index_7);
    ADD_TEST(test_geolib_point_index_8);
    ADD_TEST(test_geolib_point_index_9);

    ADD_TEST(test_geolib_contains_segment_1);
    ADD_TEST(test_geolib_contains_segment_2);
    ADD_TEST(test_geolib_contains_segment_3);
    ADD_TEST(test_geolib_contains_segment_4);
    ADD_TEST(test_geolib_contains_segment_5);
    ADD_TEST(test_geolib_contains_segment_6);

    ADD_TEST(test_geolib_union_1);
    ADD_TEST(test_geolib_union_2);
    ADD_TEST(test_geolib_union_3);
    ADD_TEST(test_geolib_union_4);
    ADD_TEST(test_geolib_union_5);
    ADD_TEST(test_geolib_union_6_extra);
    ADD_TEST(test_geolib_union_7_extra);

    ADD_TEST(test_geolib_intersection_1);
    ADD_TEST(test_geolib_intersection_2);
    ADD_TEST(test_geolib_intersection_3);
    ADD_TEST(test_geolib_intersection_4);
    ADD_TEST(test_geolib_intersection_5);

    ADD_TEST(test_geolib_difference_1);
    ADD_TEST(test_geolib_difference_2);
    ADD_TEST(test_geolib_difference_3);
    ADD_TEST(test_geolib_difference_4);
    ADD_TEST(test_geolib_difference_5);

    ADD_TEST(test_geolib_split_metropolis_1);
    ADD_TEST(test_geolib_split_metropolis_2);
    ADD_TEST(test_geolib_split_metropolis_3);
    ADD_TEST(test_geolib_split_afimoll_1);
    ADD_TEST(test_geolib_split_afimoll_2);
    ADD_TEST(test_geolib_split_afimoll_3);
    ADD_TEST(test_geolib_split_afimoll_4);
    ADD_TEST(test_geolib_split_afimoll_5);
    ADD_TEST(test_geolib_split_evropeiskyi_U1);
    ADD_TEST(test_geolib_split_evropeiskyi_1);
    ADD_TEST(test_geolib_split_evropeiskyi_2);
    ADD_TEST(test_geolib_split_evropeiskyi_3);
    ADD_TEST(test_geolib_split_evropeiskyi_4);
    ADD_TEST(test_geolib_split_dubai_mall_1);
    ADD_TEST(test_geolib_split_dubai_mall_2);
    ADD_TEST(test_geolib_split_dubai_mall_3);
    ADD_TEST(test_geolib_split_dubai_mall_4);
    ADD_TEST(test_geolib_split_dubai_mall_G);
    ADD_TEST(test_geolib_split_dubai_mall_L);
    ADD_TEST(test_geolib_split_dubai_mall_LG);

    ADD_TEST(test_geolib_buffer_1);
    ADD_TEST(test_geolib_buffer_2);
    ADD_TEST(test_geolib_buffer_3);
    ADD_TEST(test_geolib_buffer_4);
    ADD_TEST(test_geolib_buffer_5);
    ADD_TEST(test_geolib_buffer_6);

    ADD_TEST(test_geolib_buffer_metropolis_1);
    ADD_TEST(test_geolib_buffer_metropolis_2);
    ADD_TEST(test_geolib_buffer_metropolis_3);
    ADD_TEST(test_geolib_buffer_afimoll_1);
    ADD_TEST(test_geolib_buffer_afimoll_2);
    ADD_TEST(test_geolib_buffer_afimoll_3);
    ADD_TEST(test_geolib_buffer_afimoll_4);
    ADD_TEST(test_geolib_buffer_afimoll_5);
    ADD_TEST(test_geolib_buffer_evropeiskyi_U1);
    ADD_TEST(test_geolib_buffer_evropeiskyi_1);
    ADD_TEST(test_geolib_buffer_evropeiskyi_2);
    ADD_TEST(test_geolib_buffer_evropeiskyi_3);
    ADD_TEST(test_geolib_buffer_evropeiskyi_4);
    ADD_TEST(test_geolib_buffer_dubai_mall_1);
    ADD_TEST(test_geolib_buffer_dubai_mall_2);
    ADD_TEST(test_geolib_buffer_dubai_mall_3);
    ADD_TEST(test_geolib_buffer_dubai_mall_4);
    ADD_TEST(test_geolib_buffer_dubai_mall_G);
    ADD_TEST(test_geolib_buffer_dubai_mall_L);
    ADD_TEST(test_geolib_buffer_dubai_mall_LG);

    std::cout << TERMINAL_RED;
    std::cout << "Test not found: " << argv[1] << std::endl;
    std::cout << TERMINAL_DEFAULT;
    return -1;
}
