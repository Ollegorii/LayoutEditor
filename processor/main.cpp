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
    std::vector<Trapezoid> unite(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;
        size_t i = 0, j = 0;

        while (i < trapezoids1.size() && j < trapezoids2.size()) {
            const Trapezoid& trap1 = trapezoids1[i];
            const Trapezoid& trap2 = trapezoids2[j];

            if (trap1.y_bottom <= trap2.y_top) {
                // Если trap1 ниже по `y`, он не пересекается с `trap2` и может быть добавлен напрямую
                result.push_back(trap1);
                ++i;
            } else if (trap2.y_bottom <= trap1.y_top) {
                // Если trap2 ниже по `y`, он не пересекается с `trap1` и может быть добавлен напрямую
                result.push_back(trap2);
                ++j;
            } else {
                // Пересечение по y обнаружено; находим общий диапазон по y
                double y_top = std::max(trap1.y_top, trap2.y_top);
                double y_bottom = std::min(trap1.y_bottom, trap2.y_bottom);

                // Вычисляем наиболее широкие границы по x для текущего пересечения
                double x1_top = std::min(trap1.x1_top, trap2.x1_top);
                double x2_top = std::max(trap1.x2_top, trap2.x2_top);
                double x1_bottom = std::min(trap1.x1_bottom, trap2.x1_bottom);
                double x2_bottom = std::max(trap1.x2_bottom, trap2.x2_bottom);

                // Добавляем новый объединённый трапецоид с найденными координатами
                result.emplace_back(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);

                // Сдвигаем указатель на трапецоид, который заканчивается раньше
                if (trap1.y_bottom < trap2.y_bottom) {
                    ++i;
                } else {
                    ++j;
                }
            }
        }

        // Добавляем оставшиеся трапецоиды, если один из векторов был исчерпан
        while (i < trapezoids1.size()) {
            result.push_back(trapezoids1[i++]);
        }
        while (j < trapezoids2.size()) {
            result.push_back(trapezoids2[j++]);
        }

        return result;
    }


    std::vector<Trapezoid> intersect(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;
        size_t i = 0, j = 0;

        while (i < trapezoids1.size() && j < trapezoids2.size()) {
            const auto& trap1 = trapezoids1[i];
            const auto& trap2 = trapezoids2[j];

            // Проверка на пересечение по оси Y
            if (trap1.y_bottom < trap2.y_top && trap2.y_bottom < trap1.y_top) {
                // Находим диапазон Y для пересечения
                double y_bottom = std::max(trap1.y_bottom, trap2.y_bottom);
                double y_top = std::min(trap1.y_top, trap2.y_top);

                // Находим пересечение по X
                double x1_top = std::max(trap1.x1_top, trap2.x1_top);
                double x2_top = std::min(trap1.x2_top, trap2.x2_top);
                double x1_bottom = std::max(trap1.x1_bottom, trap2.x1_bottom);
                double x2_bottom = std::min(trap1.x2_bottom, trap2.x2_bottom);

                // Проверяем, что пересечение не пустое
                if (x1_top < x2_top && x1_bottom < x2_bottom) {
                    result.emplace_back(x1_bottom, x2_bottom, x1_top, x2_top, y_bottom, y_top);
                }
            }

            // Увеличиваем индекс в зависимости от высоты
            if (trap1.y_top < trap2.y_top) {
                ++i; // Переходим к следующему трапезоиду в первом векторе
            } else {
                ++j; // Переходим к следующему трапезоиду во втором векторе
            }
        }

        return result;
    }

    // Функция для вычитания двух векторов трапезоидов
    std::vector<Trapezoid> subtract(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        size_t i = 0, j = 0;

        while (i < trapezoids1.size()) {
            const auto& trap1 = trapezoids1[i];
            // Текущие границы трапезоида
            double x1_bottom_active = trap1.x1_bottom;
            double x2_bottom_active = trap1.x2_bottom;
            double x1_top_active = trap1.x1_top;
            double x2_top_active = trap1.x2_top;
            double y_top = trap1.y_top;
            double y_bottom = trap1.y_bottom;

            // Флаг для наличия пересечений
            bool has_intersection = false;

            // Обрабатываем все трапезоиды из второго вектора, которые могут пересекаться с текущим
            while (j < trapezoids2.size() && trapezoids2[j].y_bottom < y_top) {
                const auto& trap2 = trapezoids2[j];

                // Проверка на пересечение по оси Y
                if (trap1.y_bottom < trap2.y_top && trap2.y_bottom < trap1.y_top) {
                    has_intersection = true;
                    // Находим пересечение по Y
                    double intersection_y_top = std::max(y_top, trap2.y_top);
                    double intersection_y_bottom = std::min(y_bottom, trap2.y_bottom);
                    // Находим пересечение по X
                    double intersection_x1_bottom = std::max(trap1.x1_bottom, trap2.x1_bottom);
                    double intersection_x2_bottom = std::min(trap1.x2_bottom, trap2.x2_bottom);
                    double intersection_x1_top = std::max(trap1.x1_top, trap2.x1_top);
                    double intersection_x2_top = std::min(trap1.x2_top, trap2.x2_top);

                    // Добавляем непересекающиеся участки трапецоида из первого вектора в результат
                     if (x1_bottom_active < intersection_x1_bottom) {
                         result.emplace_back(x1_bottom_active, intersection_x1_bottom, x1_top_active, intersection_x1_top, intersection_y_top, intersection_y_bottom);
                     }
                     if (intersection_x2_bottom < x2_bottom_active) {
                         result.emplace_back(intersection_x2_bottom, x2_bottom_active, intersection_x2_top, x2_top_active, intersection_y_top, intersection_y_bottom);
                     }

                     // Обновляем границы текущего трапезоида по y после вычитания
                     //y_top = intersection_y_bottom;
                }
                ++j;
            }

             // Если осталась часть текущего трапезоида из первого вектора без пересечений
            if (y_top < y_bottom) {
                result.emplace_back(x1_bottom_active, x2_bottom_active, x1_top_active, x2_top_active, y_top, y_bottom);
            }

            ++i;
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
    LayerPack copyLayerFromLayerPack(const LayerPack& layerpack, const std::string& sourceLayerName, const std::string& targetLayerName) {
        LayerPack modifiedLayerPack = layerpack;

        const Layer& sourceLayer = layerpack[sourceLayerName];
        Layer copiedLayer = sourceLayer;
        copiedLayer.rename(targetLayerName);

        modifiedLayerPack.append_layer(copiedLayer);
        return modifiedLayerPack;
    }

    // Копирование слоя из одного LayerPack в другой
    LayerPack copyLayerFromLayerPack(const LayerPack& layerpack1, LayerPack& layerpack2, const std::string& sourceLayerName, const std::string& targetLayerName) {
        const Layer& sourceLayer = layerpack1[sourceLayerName];
        Layer copiedLayer = sourceLayer;
        copiedLayer.rename(targetLayerName);

        layerpack2.append_layer(copiedLayer);
        return layerpack2;
    }

    // Проверка наличия фигур в слое
    bool layerIsEmpty(const Layer& layer) {
        return !layer.get_polygons().empty();
    }
}  // namespace LayerOperations

int main()
{
    return 0;
}
