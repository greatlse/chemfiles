Chemfiles, an efficient IO library for chemistry file formats
=============================================================

Chemfiles is a multi-language library written in modern C++ for reading and writing
from and to molecular trajectory files. These files are created by your favorite
theoretical chemistry program, and contains informations about atomic or residues
names and positions. Some format also have additional informations, such as
velocities, forces, energy, …

The main targeted audience of chemfiles (*libchemfiles*) are chemistry researchers
working on their own code to do some kind of awesome science, without wanting to
bother about handling all the format that may exist in the world.

Running simulation (either Quantum Dynamic, Monte Carlo, Molecular Dynamic, or
any other method) often produce enormous amounts of data, which had to be
post-processed in order to extract informations. This post-processing step involve
reading and parsing the data, and computing physical values with the help of
statistical thermodynamic. Chemfiles tries to help you on the first point, by providing
the same interface to all the trajectory formats. If you ever need to change your
output format, your analysis tools will still work the same way. Chemfiles is
efficient because it allow you to write and debug your code only once, and then
to re-use it as needed.

.. note::

    Chemfiles is still is alpha stage, and no backward compatibility is assured.
    I hope I can reach a stable interface pretty soon, once it have been validated on
    various formats.

User manual
-----------

Even if chemfiles is written in C++, it can be used from the most popular scientific
programming languages: C, Fortran, Python, … You can just pick up your favorite
language to use it. This part of the documentation presents the data model used by
chemfiles to store information about the trajectories, and how to acess that data in
C and C++.

Basic usage of chemfiles looks like this in C++:

.. code-block:: cpp

    #include <iostream>

    #include "chemfiles.cpp"
    using namespace chemfiles;

    int main() {
        Trajectory traj("filename.xyz");
        Frame frame;

        traj >> frame;
        std::cout << "There are " << frame.natoms() << " atoms in the frame" << std::endl;
        auto positions = frame.positions();

        // Do awesome science here with the positions

        if (frame.has_velocities()) {
            auto velocities = frame.velocities();

            // If the file contains information about the velocities, you will
            // find them here.
        }
    }

In the following parts, the documentation will cover how to install chemfiles, and
how the data are organised.

.. toctree::
    :maxdepth: 2

    installation
    overview
    example
    formats
    others

.. _classes-reference:

Classes and functions reference
-------------------------------

All the public classes (for the C++ interface) and functions (for the C interface)
are extensively documented here.

.. toctree::
   :maxdepth: 2

   classes/index
   c-api

Developer documentation
-----------------------

.. toctree::
   :maxdepth: 2

   devdoc/contributing
   devdoc/internals
   devdoc/file
   devdoc/format
