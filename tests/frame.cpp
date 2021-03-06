#include "catch.hpp"
#include "chemfiles.hpp"
#include "chemfiles/Frame.hpp"
#include "chemfiles/UnitCell.hpp"
using namespace chemfiles;

TEST_CASE("Frame class usage", "[Frame]"){
    auto frame = Frame(10);

    SECTION("Contructor"){
        CHECK(frame.positions().capacity() == 10);
        CHECK(frame.cell().type() == UnitCell::INFINITE);
    }

    SECTION("Get and set"){
        CHECK(frame.step() == 0);
        frame.step(1000);
        CHECK(frame.step() == 1000);

        frame.cell(UnitCell(10));
        CHECK(frame.cell().type() == UnitCell::ORTHOROMBIC);

        CHECK(frame.topology().natom_types() == 0);
        frame.topology().append(Atom("H"));
        CHECK(frame.topology().natom_types() == 1);

        frame.resize(15);
        CHECK(frame.natoms() == 15);
        // No velocity data yet
        CHECK_FALSE(frame.has_velocities());

        frame.resize(15, true);
        CHECK(frame.has_velocities());

        frame.positions()[0] = Vector3D(1, 2, 3);
        CHECK(frame.positions()[0] == Vector3D(1, 2, 3));
        frame.velocities()[0] = Vector3D(5, 6, 7);
        CHECK(frame.velocities()[0] == Vector3D(5, 6, 7));

        auto mat = new float[15][3];
        frame.raw_positions(mat, 15);
        CHECK(mat[0][0] == 1);
        CHECK(mat[0][1] == 2);
        CHECK(mat[0][2] == 3);

        frame.raw_velocities(mat, 15);
        CHECK(mat[0][0] == 5);
        CHECK(mat[0][1] == 6);
        CHECK(mat[0][2] == 7);

        delete[] mat;

        Array3D data;
        for (size_t i=0; i<10; i++)
            data.push_back(Vector3D(4.f, 3.4f, 1.f));
        frame.positions(data);
        frame.velocities(data);

        auto positions = frame.positions();
        auto velocities = frame.velocities();

        for (size_t i=0; i<10; i++){
            CHECK(positions[i] == Vector3D(4.f, 3.4f, 1.f));
            CHECK(velocities[i] == Vector3D(4.f, 3.4f, 1.f));
        }
    }

    SECTION("Errors"){
        auto mat = new float[3][3];

        CHECK_THROWS_AS(frame.raw_positions(mat, 3), MemoryError);

        // This is not throwing, but only filling the array with zeroes
        frame.raw_velocities(mat, 3);
        for (size_t i=0; i<3; i++)
            for (size_t j=0; j<3; j++)
                CHECK(mat[i][j] == 0);

        frame.resize(10, true);
        CHECK_THROWS_AS(frame.raw_velocities(mat, 3), MemoryError);
        delete[] mat;
    }

    SECTION("Guess bonds"){
        Trajectory file(SRCDIR "/data/xyz/methane.xyz");

        file >> frame;
        frame.guess_topology();

        auto topology =  frame.topology();
        for (size_t i=1; i<5; i++){
            CHECK(topology.isbond(0, i));
        }

        CHECK_FALSE(topology.isbond(2, 4));
        CHECK_FALSE(topology.isbond(1, 2));

        CHECK(topology.isangle(1, 0, 2));
        CHECK(topology.isangle(3, 0, 2));
        CHECK(topology.isangle(2, 0, 4));

        CHECK(topology.bonds().size() == 4);
    }
}
