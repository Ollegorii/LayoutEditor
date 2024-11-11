#include <iostream>
#include <vector>
#include <cmath>
#include "GeometryOperations.h"

const double EPSILON = 1e-6;

bool are_trapezoids_equal(const Trapezoid& t1, const Trapezoid& t2) {
    return std::abs(t1.x1_top - t2.x1_top) < EPSILON &&
           std::abs(t1.x2_top - t2.x2_top) < EPSILON &&
           std::abs(t1.x1_bottom - t2.x1_bottom) < EPSILON &&
           std::abs(t1.x2_bottom - t2.x2_bottom) < EPSILON &&
           std::abs(t1.y_top - t2.y_top) < EPSILON &&
           std::abs(t1.y_bottom - t2.y_bottom) < EPSILON;
}

bool are_vectors_equal(const std::vector<Trapezoid>& v1, const std::vector<Trapezoid>& v2) {
    if (v1.size() != v2.size()) return false;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (!are_trapezoids_equal(v1[i], v2[i])) {
            return false;
        }
    }
    return true;
}

void assert_equal(const std::vector<Trapezoid>& result, const std::vector<Trapezoid>& expected, const std::string& test_name) {
    if (are_vectors_equal(result, expected)) {
        std::cout << test_name << " passed.\n";
    } else {
        std::cout << test_name << " failed.\n";
    }
}


void test_unite() {
    Trapezoid t1(0, 2, 0, 2, 2, 0); // Квадратный трапецоид
    Trapezoid t2(1, 3, 1, 3, 3, 1); // Наложен частично

    // Ожидаемый результат для объединения этих трапецоидов
    std::vector<Trapezoid> expected_unite = {
        Trapezoid(0, 2, 0, 2, 1, 0), // Верхняя часть t1
        Trapezoid(0, 3, 0, 3, 2, 1), // Перекрывающая часть
        Trapezoid(1, 3, 1, 3, 3, 2)  // Нижняя часть t2
    };

    std::vector<Trapezoid> result_unite = TrapezoidOperations::unite({t1}, {t2});
    std::cout << "Unite = ";
    for(auto trap : result_unite)
    {
        std::cout << trap.x1_top << " " << trap.x2_top << " " << trap.x1_bottom << " " << trap.x2_bottom << " " << trap.y_top << " " << trap.y_bottom << "\n";
    }
    assert_equal(result_unite, expected_unite, "Unite Test");
}

void test_intersect() {
    Trapezoid t1(0, 2, 0, 2, 2, 0); // Квадратный трапецоид
    Trapezoid t2(1, 3, 1, 3, 3, 1); // Наложен частично

    // Ожидаемый результат для пересечения
    std::vector<Trapezoid> expected_intersect = {
        Trapezoid(1, 2, 1, 2, 2, 1)
    };


    std::vector<Trapezoid> result_intersect = TrapezoidOperations::intersect({t1}, {t2});
    std::cout << "Intersect = ";
    for(auto trap : result_intersect)
    {
        std::cout << trap.x1_top << " " << trap.x2_top << " " << trap.x1_bottom << " " << trap.x2_bottom << " " << trap.y_top << " " << trap.y_bottom << "\n";
    }
    assert_equal(result_intersect, expected_intersect, "Intersect Test");
}

void test_subtract() {
    Trapezoid t1(0, 2, 0, 2, 2, 0); // Квадратный трапецоид
    Trapezoid t2(1, 3, 1, 3, 3, 1); // Наложен частично

    // Ожидаемый результат для вычитания
    std::vector<Trapezoid> expected_subtract = {
        Trapezoid(0, 2, 0, 2, 1, 0),
        Trapezoid(0, 1, 0, 1, 2, 1)
    };

    std::vector<Trapezoid> result_subtract = TrapezoidOperations::subtract({t1}, {t2});
    std::cout << "Substruct = ";
    for(auto trap : result_subtract)
    {
        std::cout << trap.x1_top << " " << trap.x2_top << " " << trap.x1_bottom << " " << trap.x2_bottom << " " << trap.y_top << " " << trap.y_bottom << "\n";
    }
    assert_equal(result_subtract, expected_subtract, "Subtract Test");
}

//void test_copy_layer() {
//    LayerPack layerpack;
//    layerpack.addLayer("Layer1", {Trapezoid(0, 2, 0, 2, 0, 2)});

//    LayerPack copied_layerpack = LayerOperations::copyLayerFromLayerPack(layerpack, "Layer1", "CopiedLayer");

//    assert_equal(copied_layerpack.getLayer("CopiedLayer"), layerpack.getLayer("Layer1"), "Copy Layer Test");
//}

//void test_modifyPolygon() {
//    std::vector<Polygon> polygons = {
//        Polygon({{0, 0}, {1, 0}, {1, 1}, {0, 1}}), // Квадратный полигон
//    };

//    // Масштабируем в 2 раза
//    float scale_factor = 2.0;
//    std::vector<Polygon> expected_modified = {
//        Polygon({{-1, -1}, {2, -1}, {2, 2}, {-1, 2}})
//    };

//    std::vector<Polygon> result_modified = PolygonOperations::modifyPolygon(polygons, scale_factor);
//    bool success = true;

//    for (size_t i = 0; i < result_modified.size(); ++i) {
//        if (result_modified[i] != expected_modified[i]) {
//            success = false;
//            break;
//        }
//    }

//    if (success) {
//        std::cout << "modifyPolygon Test passed.\n";
//    } else {
//        std::cout << "modifyPolygon Test failed.\n";
//    }
//}

int main() {
    test_unite();
    test_intersect();
    test_subtract();
    //test_copy_layer();
    //test_modifyPolygon();
    return 0;
}
