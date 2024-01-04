#ifndef LINEARALGEBRACURSE_EXCEPTIONS_H
#define LINEARALGEBRACURSE_EXCEPTIONS_H

#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>

namespace LinearAlgebra {

    class NormalizeZeroVectorException : public std::exception {
        virtual const char *what() const throw() {
            return "Cannot normalize the zero vector.";
        }
    };

    class AngleWithZeroVectorException : public std::exception {
        virtual const char *what() const throw() {
            return "Cannot compute an angle with the zero vector.";
        }
    };

    class NoUniqueParallelComponentException : public std::exception {
        virtual const char *what() const throw() {
            return "There is no unique parallel component.";
        }
    };

    class NoUniqueOrthogonalComponentException : public std::exception {
        virtual const char *what() const throw() {
            return "There is no unique orthogonal component.";
        }
    };

    class CrossProductException {
    public:
        virtual const char *what() const throw() = 0;
    };

    class NoCrossProductForDimensionException : public CrossProductException, public std::exception {
        virtual const char *what() const throw() {
            return "Cross product only defined for 3 Or 2 dimensions.";
        }
    };

    class SomethingWrongInCrossProductException : public CrossProductException, public std::exception {
        virtual const char *what() const throw() {
            return "The test part for orthogonality throws error.";
        }
    };

    class Needed3DimForCrossProductException : public CrossProductException, public std::exception {
        virtual const char *what() const throw() {
            return "Needed 3 dimensional vectors to compute cross product.";
        }
    };

    class VectorNotExistsInSpecifiedDimensionException : public std::exception {
        virtual const char *what() const throw() {
            return "This vector does not exists in the specified dimension.";
        }
    };

    class CantComputeParallelogramException : public std::exception {
    private:
        CrossProductException *cpe;

    public:
        CantComputeParallelogramException(CrossProductException &cpe) : cpe(&cpe) {}

        virtual const char *what() const throw() {
            std::stringstream s;
            s << "Can't compute parallelogram, due to cross product error: " << cpe->what();
            return s.str().c_str();
        }
    };

    class NoNonzeroElementInVecException : public std::exception {
        virtual const char *what() const throw() {
            return "There is no nonzero element in the vector.";
        }
    };

    class NoIntersectionPointException : public std::exception {
        virtual const char *what() const throw() {
            return "Parallel lines have no intersection point.";
        }
    };

    class InfiniteIntersectionsPointsException : public std::exception {
        virtual const char *what() const throw() {
            return "Same lines have infinite intersections points.";
        }
    };

    class ThereIsNoSuchDimensionInSystem : public std::exception {
        virtual const char *what() const throw() {
            return "Requested dimension is too high for this system.";
        }
    };

    class planesInDifferentDimException : public std::exception {
        virtual const char* what() const throw()
        {
            return "All planes in the system should live in the same dimension.";
        }
    };

    class rowsInDifferentDimException : public std::exception {
        virtual const char* what() const throw()
        {
            return "All rows in matrix should live in the same dimension.";
        }
    };

    class hyperplanesInDifferentDimException : public std::exception {
        virtual const char* what() const throw()
        {
            return "All hyper-planes in the system should live in the same dimension.";
        }
    };

    class rowNotExistsInMatrixException : public std::exception {
        virtual const char* what() const throw()
        {
            return "This row does not exists in this matrix.";
        }
    };

    class illegalRowException : public std::exception {
        virtual const char* what() const throw()
        {
            return "Specified row is illegal.";
        }
    };

    class NotImplementedError : public std::exception {
        virtual const char* what() const throw()
        {
            return "This feature does not implemented yet.";
        }
    };
}

#endif //LINEARALGEBRACURSE_EXCEPTIONS_H