#include "chemfiles.h"

// Force NDEBUG to be undefined
#undef NDEBUG
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool roughly(double A[3][3], double B[3][3]) {
    double eps = 1e-10;
    return
        (fabs(A[0][0] - B[0][0]) < eps) && (fabs(A[0][1] - B[0][1]) < eps) && (fabs(A[0][2] - B[0][2]) < eps) &&
        (fabs(A[1][0] - B[1][0]) < eps) && (fabs(A[1][1] - B[1][1]) < eps) && (fabs(A[1][2] - B[1][2]) < eps) &&
        (fabs(A[2][0] - B[2][0]) < eps) && (fabs(A[2][1] - B[2][1]) < eps) && (fabs(A[2][2] - B[2][2]) < eps);
}

int main() {
    CHFL_CELL* cell = chfl_cell(2, 3, 4);

    double a=0, b=0, c=0, V=0;
    assert(!chfl_cell_lengths(cell, &a, &b, &c));
    assert(fabs(a - 2) < 1e-10);
    assert(fabs(b - 3) < 1e-10);
    assert(fabs(c - 4) < 1e-10);

    assert(!chfl_cell_angles(cell, &a, &b, &c));
    assert(fabs(a - 90) < 1e-10);
    assert(fabs(b - 90) < 1e-10);
    assert(fabs(c - 90) < 1e-10);

    assert(!chfl_cell_volume(cell, &V));
    assert(fabs(V - 2*3*4) < 1e-10);

    assert(!chfl_cell_set_lengths(cell, 10, 20, 30));
    assert(!chfl_cell_lengths(cell, &a, &b, &c));
    assert(fabs(a - 10) < 1e-10);
    assert(fabs(b - 20) < 1e-10);
    assert(fabs(c - 30) < 1e-10);

    chfl_set_loglevel(CHFL_LOG_NONE);
    // This should be an error
    assert(chfl_cell_set_angles(cell, 80, 89, 100));
    chfl_set_loglevel(CHFL_LOG_ERROR);

    double expected_mat[3][3] = {{10, 0, 0}, {0, 20, 0}, {0, 0, 30}};
    double mat[3][3];
    assert(!chfl_cell_matrix(cell, mat));
    assert(roughly(expected_mat, mat));

    chfl_cell_type_t type;
    assert(!chfl_cell_type(cell, &type));
    assert(type == CHFL_CELL_ORTHOROMBIC);

    assert(!chfl_cell_set_type(cell, CHFL_CELL_TRICLINIC));
    assert(!chfl_cell_type(cell, &type));
    assert(type == CHFL_CELL_TRICLINIC);

    assert(!chfl_cell_set_angles(cell, 80, 89, 100));
    assert(!chfl_cell_angles(cell, &a, &b, &c));
    assert(fabs(a - 80) < 1e-10);
    assert(fabs(b - 89) < 1e-10);
    assert(fabs(c - 100) < 1e-10);

    bool x=false, y=false, z=false;
    assert(!chfl_cell_periodicity(cell, &x, &y, &z));
    assert(x == true);
    assert(y == true);
    assert(z == true);

    assert(!chfl_cell_set_periodicity(cell, false, true, false));
    assert(!chfl_cell_periodicity(cell, &x, &y, &z));
    assert(x == false);
    assert(y == true);
    assert(z == false);

    assert(!chfl_cell_free(cell));

    return EXIT_SUCCESS;
}
