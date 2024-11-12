#include <algorithm>
#include <utility>
#include <tuple>
#include "GeometryOperations.h"

Trapezoid :: Trapezoid(double x1_top, double x2_top, double x1_bottom, double x2_bottom, double y_top, double y_bottom)
        : x1_top(x1_top), x2_top(x2_top), x1_bottom(x1_bottom), x2_bottom(x2_bottom), y_top(y_top), y_bottom(y_bottom) {}

    // Копирующий конструктор
Trapezoid :: Trapezoid(const Trapezoid& other)
        : x1_top(other.x1_top), x2_top(other.x2_top), x1_bottom(other.x1_bottom), x2_bottom(other.x2_bottom),
          y_top(other.y_top), y_bottom(other.y_bottom) {}

    // Конструктор перемещения
Trapezoid :: Trapezoid(Trapezoid&& other)
        : x1_top(std::move(other.x1_top)), x2_top(std::move(other.x2_top)),
          x1_bottom(std::move(other.x1_bottom)), x2_bottom(std::move(other.x2_bottom)),
          y_top(std::move(other.y_top)), y_bottom(std::move(other.y_bottom)) {}


Trapezoid& Trapezoid :: operator=(const Trapezoid& other) {
        if (this != &other) { // проверка самоприсваивания
            x1_top = other.x1_top;
            x2_top = other.x2_top;
            x1_bottom = other.x1_bottom;
            x2_bottom = other.x2_bottom;
            y_top = other.y_top;
            y_bottom = other.y_bottom;
        }
        return *this;
    }



namespace TrapezoidOperations {

    // Функция для вычисления пересечения двух трапецоидов по оси y
    std::pair<double, double> overlapY(const Trapezoid& a, const Trapezoid& b) {
        double y_top = std::min(a.y_top, b.y_top);
        double y_bottom = std::max(a.y_bottom, b.y_bottom);
        return std::make_pair(y_top, y_bottom);
    }

    // Линейная интерполяция для вычисления координаты x на высоте y
    double interpolateX(double y, double y1, double y2, double x1, double x2) {
        return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    }

    // Создает трапецоид для области пересечения,
    // вычисляя актуальные координаты трапецоида для рассматриваемой координаты y
    Trapezoid create_trapezoid(double y_top, double y_bottom, const Trapezoid& a, const Trapezoid& b, bool is_union) {
        double x1_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        double x1_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        double x1_top = is_union ? std::min(x1_top_a, x1_top_b)
                                 : std::max(x1_top_a, x1_top_b);

        double x2_top = is_union ? std::max(x2_top_a, x2_top_b)
                                 : std::min(x2_top_a, x2_top_b);

        double x1_bottom = is_union ? std::min(x1_bottom_a, x1_bottom_b)
                                    : std::max(x1_bottom_a, x1_bottom_b);


        double x2_bottom = is_union ? std::max(x2_bottom_a, x2_bottom_b)
                                    : std::min(x2_bottom_a, x2_bottom_b);

        return Trapezoid(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);

    }

    std::vector<Trapezoid> create_trapezoids_substructed(double y_top, double y_bottom, const Trapezoid& a, const Trapezoid& b, bool is_substruct=false){
        std::vector<Trapezoid> result;
        double x1_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        double x1_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        if(a.x1_bottom <= b.x1_bottom){
            double x1_top = x1_top_a;
            double x2_top =  std::min(x2_top_a, x1_top_b);
            double x1_bottom = x1_bottom_a;
            double x2_bottom = std::min(x2_bottom_a, x1_bottom_b);
            result.emplace_back(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);
        }
        else if(a.x2_bottom >= b.x2_bottom){
            double x1_top = std::max(x1_top_a, x2_top_b);
            double x2_top =  x2_top_a;
            double x1_bottom = std::max(x1_bottom_a, x2_bottom_b);
            double x2_bottom = x2_bottom_a;
            result.emplace_back(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);
        }
        return result;
    }


    std::vector<Trapezoid> unite(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        for (const auto& a : trapezoids1) {
            bool intersected = false;

            for (const auto& b : trapezoids2) {
                std::pair<double, double> y_overlap_result = overlapY(a, b);
                double y_top = y_overlap_result.first;
                double y_bottom = y_overlap_result.second;

                if (y_top > y_bottom) { // Есть пересечение по y
                    // 1. Добавляем верхнюю часть трапецоида до области пересечения
                    if (a.y_bottom < y_bottom) {
                        result.push_back(create_trapezoid(y_bottom, a.y_bottom, a, a, true));
                    } else if (b.y_bottom < y_bottom) {
                        result.push_back(create_trapezoid(y_bottom, b.y_bottom, b, b, true));
                    }

                    // 2. Добавляем общую часть пересечения (объединение трапецоидов)
                    result.push_back(create_trapezoid(y_top, y_bottom, a, b, true));

                    // 3. Добавляем нижнюю часть трапецоида после области пересечения
                    if (a.y_top > y_top) {
                        result.push_back(create_trapezoid(a.y_top, y_top, a, a, true));
                    } else if (b.y_top > y_top) {
                        result.push_back(create_trapezoid(b.y_top, y_top, b, b, true));
                    }

                    intersected = true;
                    break;
                }
            }

            // Если не было пересечения, добавляем весь трапецоид
            if (!intersected) {
                result.push_back(a);
            }
        }

        // Добавляем трапецоиды из второго множества, которые не пересеклись с первым
        for (const auto& b : trapezoids2) {
            bool intersected = false;

            for (const auto& a : trapezoids1) {
                std::pair<double, double> y_overlap_result = overlapY(b, a);
                if (y_overlap_result.first > y_overlap_result.second) {
                    intersected = true;
                    break;
                }
            }

            if (!intersected) {
                result.push_back(b);
            }
        }

        return result;
    }


    std::vector<Trapezoid> intersect(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        for (const auto& a : trapezoids1) {
            for (const auto& b : trapezoids2) {
                // Проверяем пересечение по y
                std::pair<double, double> y_overlap_result = overlapY(a, b);
                double y_top = y_overlap_result.first;
                double y_bottom = y_overlap_result.second;

                if (y_top > y_bottom) { // Есть пересечение по y
                    // Добавляем только общую часть, используя `create_trapezoid`
                    result.push_back(create_trapezoid(y_top, y_bottom, a, b, false));  // false для пересечения
                }
            }
        }

        return result;
    }


    // Функция для вычитания двух векторов трапезоидов
    std::vector<Trapezoid> subtract(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        for (const auto& a : trapezoids1) {
            bool intersected = false;

            for (const auto& b : trapezoids2) {
                // Проверяем пересечение по y
                std::pair<double, double> y_overlap_result = overlapY(a, b);
                double y_top = y_overlap_result.first;
                double y_bottom = y_overlap_result.second;

                if (y_top > y_bottom) { // Есть пересечение по y
                    intersected = true;

                    // 1. Добавляем часть до пересечения
                    if (a.y_bottom < y_bottom) {
                        result.push_back(create_trapezoid(y_bottom, a.y_bottom, a, a, true));
                    }

                    // 2. Переcечение
                    auto trapezoids = create_trapezoids_substructed(y_top, y_bottom, a, b, true);
                    for(const auto& trap : trapezoids)
                        result.push_back(trap);

                    // 3. Добавляем часть после пересечения
                    if (a.y_top > y_top) {
                        result.push_back(create_trapezoid(a.y_top, y_top, a, a, true));
                    }
                    break;
                }
            }

            // Если не было пересечения, добавляем весь трапецоид из первого множества
            if (!intersected) {
                result.push_back(a);
            }
        }

        return result;
    }
} // namespace TrapezoidOperations



namespace PolygonOperations {

     std::vector<Polygon> modifyPolygon(const std::vector<Polygon>& polygons, float size) {
        std::vector<Polygon> modifiedPolygons;

        size = size > 0 ? size : 1 / std::abs(size); // Если size - отрицаетльный, то делим на модуль size

        for (const Polygon& polygon : polygons) {
            std::vector<Point> newVertices;

            // Изменяем каждую вершину, масштабируя её относительно центра полигона
            Point center = Point(0, 0);
            for (const Point& vertex : polygon.get_vertices()) {
                center = center + vertex;
            }
            center.x /= polygon.get_vertices().size();
            center.y /= polygon.get_vertices().size();

            for (const Point& vertex : polygon.get_vertices()) {
                Point scaledVertex = center + (vertex - center) * size;
                newVertices.push_back(scaledVertex);
            }
            // Масштабируем каждую вершину для каждой дырки относительно центра полигона
            std::vector<Hole> newHoles;
            for (const Hole& hole : polygon.get_holes()) {
                std::vector<Point> newHoleVertices;
                for (const Point& vertex : hole.get_vertices()) {
                    double newX = center.x + (vertex.x - center.x) * size;
                    double newY = center.y + (vertex.y - center.y) * size;
                    newHoleVertices.emplace_back(newX, newY);
                }
                newHoles.emplace_back(newHoleVertices);
            }

            // Создаём новый полигон с модифицированными вершинами и дырками
            Polygon modifiedPolygon(newVertices, newHoles);
            modifiedPolygons.push_back(modifiedPolygon);
        }

        return modifiedPolygons;
    }
}   // namespace PolygonOperations

namespace LayerOperations {

    // Копирование слоя внутри одного LayerPack
    void copyLayerFromLayerPack(LayerPack& layerpack, const std::string& sourceLayerName, const std::string& targetLayerName) {

        const Layer& sourceLayer = layerpack[sourceLayerName];
        Layer copiedLayer = sourceLayer;
        copiedLayer.rename(targetLayerName);

        layerpack.append_layer(copiedLayer);
    }

    // Копирование слоя из одного LayerPack в другой
    void copyLayerFromLayerPack(const LayerPack& layerpack1, LayerPack& layerpack2, const std::string& sourceLayerName, const std::string& targetLayerName) {
        const Layer& sourceLayer = layerpack1[sourceLayerName];
        Layer copiedLayer = sourceLayer;
        copiedLayer.rename(targetLayerName);

        layerpack2.append_layer(copiedLayer);
    }

    // Проверка наличия фигур в слое
    bool layerIsEmpty(const Layer& layer) {
        return !layer.get_polygons().empty();
    }
}  // namespace LayerOperations

