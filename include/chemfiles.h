/* Chemfiles, an efficient IO library for chemistry file formats
* Copyright (C) 2015 Guillaume Fraux
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/
*/

/*! @file chemfiles.h
* Chemfiles C interface header.
*
* This file contains all the function definitions for the C API of chemfiles, and
* should be self-documented enough.
*/

#ifndef CHEMFILES_CAPI_H
#define CHEMFILES_CAPI_H

#define CHEMFILES_PUBLIC
    #include "chemfiles/config.hpp"
#undef CHEMFILES_PUBLIC

#ifdef __cplusplus
    #include <cstddef>
extern "C" {

    namespace chemfiles {
        class Trajectory;
        class Frame;
        class Atom;
        class UnitCell;
        class Topology;
    }
    typedef chemfiles::Trajectory CHFL_TRAJECTORY;
    typedef chemfiles::Frame CHFL_FRAME;
    typedef chemfiles::Atom CHFL_ATOM;
    typedef chemfiles::UnitCell CHFL_CELL;
    typedef chemfiles::Topology CHFL_TOPOLOGY;
#else
    #include <stddef.h>
    #include <stdbool.h>
    //! Opaque type handling trajectories files
    typedef struct CHFL_TRAJECTORY CHFL_TRAJECTORY;
    //! Opaque type handling frames, *i.e* data from a step
    typedef struct CHFL_FRAME CHFL_FRAME;
    //! Opaque type handling an atom
    typedef struct CHFL_ATOM CHFL_ATOM;
    //! Opaque type handling an unit cell
    typedef struct CHFL_CELL CHFL_CELL;
    //! Opaque type handling a topology
    typedef struct CHFL_TOPOLOGY CHFL_TOPOLOGY;
#endif

/*!
* @brief Get the error message corresponding to an error code.
* @param status The error code
* @return A null-terminated string encoding the textual representation of the status.
*/
CHFL_EXPORT const char* chfl_strerror(int status);

/*!
* @brief Get the last error message.
* @return A null-terminated string encoding the textual representation of the last error.
*/
CHFL_EXPORT const char* chfl_last_error();

//! Available logging level
typedef enum CHFL_LOG_LEVEL {
    //! Do not log anything
    CHFL_LOG_NONE = 0,
    //! Only log on errors
    CHFL_LOG_ERROR = 1,
    //! Log warnings and erors
    CHFL_LOG_WARNING = 2,
    //! Log infos, warnings and errors
    CHFL_LOG_INFO = 3,
    //! Log everything
    CHFL_LOG_DEBUG = 4
} chfl_log_level_t ;

/*!
* @brief Get the current logging level
* @param level The logging level
* @return The status code
*/
CHFL_EXPORT int chfl_loglevel(chfl_log_level_t* level);

/*!
* @brief Set the current logging level to \c level
* @param level The new logging level
* @return The status code
*/
CHFL_EXPORT int chfl_set_loglevel(chfl_log_level_t level);

/*!
* @brief Redirect the logs to \c file, overwriting the file if it exists
* @param file The filename for the new log file.
* @return The status code
*/
CHFL_EXPORT int chfl_logfile(const char* file);

/*!
* @brief Redirect the logs to the standard error output. This is enabled by default.
* @return The status code
*/
CHFL_EXPORT int chfl_log_stderr();

/******************************************************************************/
/*!
* @brief Open a trajectory file.
* @param filename The path to the trajectory file
* @param mode The opening mode: "r" for read, "w" for write and  "a" for append.
* @return A pointer to the file
*/
CHFL_EXPORT CHFL_TRAJECTORY* chfl_trajectory_open(const char* filename, const char* mode);

/*!
* @brief Open a trajectory file using a given format to read the file.
* @param filename The path to the trajectory file
* @param mode The opening mode: "r" for read, "w" for write and  "a" for append.
* @param format The format to use
* @return A pointer to the file
*/
CHFL_EXPORT CHFL_TRAJECTORY* chfl_trajectory_with_format(const char* filename, const char* mode, const char* format);

/*!
* @brief Read the next step of the trajectory into a frame
* @param file A pointer to the trajectory
* @param frame A frame to fill with the data
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_read(CHFL_TRAJECTORY *file, CHFL_FRAME *frame);

/*!
* @brief Read a specific step of the trajectory in a frame
* @param file A pointer to the trajectory
* @param step The step to read
* @param frame A frame to fill with the data
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_read_step(CHFL_TRAJECTORY *file, size_t step, CHFL_FRAME* frame);

/*!
* @brief Write a frame to the trajectory.
* @param file The trajectory to write
* @param frame the frame which will be writen to the file
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_write(CHFL_TRAJECTORY *file, const CHFL_FRAME *frame);

/*!
* @brief Set the topology associated with a trajectory. This topology will be
*        used when reading and writing the files, replacing any topology in the
*        frames or files.
* @param file A pointer to the trajectory
* @param topology The new topology to use
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_set_topology(CHFL_TRAJECTORY *file, const CHFL_TOPOLOGY *topology);

/*!
* @brief Set the topology associated with a trajectory by reading the first
*        frame of \c filename; and extracting the topology of this frame.
* @param file A pointer to the trajectory
* @param filename The file to read in order to get the new topology
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_set_topology_file(CHFL_TRAJECTORY *file, const char* filename);

/*!
* @brief Set the unit cell associated with a trajectory. This cell will be
*        used when reading and writing the files, replacing any unit cell in the
*        frames or files.
* @param file A pointer to the trajectory
* @param cell The new cell to use
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_set_cell(CHFL_TRAJECTORY *file, const CHFL_CELL *cell);

/*!
* @brief Get the number of steps (the number of frames) in a trajectory.
* @param file A pointer to the trajectory
* @param nsteps This will contain the number of steps
* @return The status code.
*/
CHFL_EXPORT int chfl_trajectory_nsteps(CHFL_TRAJECTORY *file, size_t *nsteps);

/*!
* @brief Synchronize any buffered content to the hard drive.
* @param file A pointer to the file
* @return The status code
*/
CHFL_EXPORT int chfl_trajectory_sync(CHFL_TRAJECTORY *file);

/*!
* @brief Close a trajectory file, and free the associated memory
* @param file A pointer to the file
* @return The status code
*/
CHFL_EXPORT int chfl_trajectory_close(CHFL_TRAJECTORY *file);

/******************************************************************************/
/*!
* @brief Create an empty frame with initial capacity of \c natoms. It will be
*        resized by the library as needed.
* @param natoms the size of the wanted frame
* @return A pointer to the frame
*/
CHFL_EXPORT CHFL_FRAME* chfl_frame(size_t natoms);

/*!
* @brief Get the current number of atoms in the frame.
* @param frame The frame to analyse
* @param natoms the number of atoms in the frame
* @return The status code
*/
CHFL_EXPORT int chfl_frame_atoms_count(const CHFL_FRAME* frame, size_t *natoms);

/*!
* @brief Get the positions from a frame
* @param frame The frame
* @param data A Nx3 float array to be filled with the data
* @param size The array size (N).
* @return The status code
*/
CHFL_EXPORT int chfl_frame_positions(const CHFL_FRAME* frame, float (*data)[3], size_t size);

/*!
* @brief Set the positions of a frame
* @param frame The frame
* @param data A Nx3 float array containing the positions in row-major order.
* @param size The array size (N).
* @return The status code
*/
CHFL_EXPORT int chfl_frame_set_positions(CHFL_FRAME* frame, float const (*data)[3], size_t size);

/*!
* @brief Get the velocities from a frame, if they exists
* @param frame The frame
* @param data A Nx3 float array to be filled with the data
* @param size The array size (N).
* @return The status code
*/
CHFL_EXPORT int chfl_frame_velocities(const CHFL_FRAME* frame, float (*data)[3], size_t size);

/*!
* @brief Set the velocities of a frame.
* @param frame The frame
* @param data A Nx3 float array containing the velocities in row-major order.
* @param size The array size (N).
* @return The status code
*/
CHFL_EXPORT int chfl_frame_set_velocities(CHFL_FRAME* frame, float const (*data)[3], size_t size);

/*!
* @brief Check if a frame has velocity information.
* @param frame The frame
* @param has_vel true if the frame has velocities, false otherwise.
* @return The status code
*/
CHFL_EXPORT int chfl_frame_has_velocities(const CHFL_FRAME* frame, bool *has_vel);

/*!
* @brief Set the UnitCell of a Frame.
* @param frame The frame
* @param cell The new cell
* @return The status code
*/
CHFL_EXPORT int chfl_frame_set_cell(CHFL_FRAME* frame, const CHFL_CELL* cell);

/*!
* @brief Set the Topology of a Frame.
* @param frame The frame
* @param topology The new topology
* @return The status code
*/
CHFL_EXPORT int chfl_frame_set_topology(CHFL_FRAME* frame, const CHFL_TOPOLOGY* topology);

/*!
* @brief Get the Frame step, i.e. the frame number in the trajectory
* @param frame The frame
* @param step This will contains the step number
* @return The status code
*/
CHFL_EXPORT int chfl_frame_step(const CHFL_FRAME* frame, size_t* step);

/*!
* @brief Set the Frame step.
* @param frame The frame
* @param step The new frame step
* @return The status code
*/
CHFL_EXPORT int chfl_frame_set_step(CHFL_FRAME* frame, size_t step);

/*!
* @brief Try to guess the bonds, angles and dihedrals in the system. If \c bonds
*        is true, guess everything; else only guess the angles and dihedrals from
*        the topology bond list.
* @param frame The Frame to analyse
* @param bonds Should we recompute the bonds from the positions or not ?
* @return The status code
*/
CHFL_EXPORT int chfl_frame_guess_topology(CHFL_FRAME* frame, bool bonds);

/*!
* @brief Destroy a frame, and free the associated memory
* @param frame The frame to destroy
* @return The status code
*/
CHFL_EXPORT int chfl_frame_free(CHFL_FRAME* frame);

/******************************************************************************/
/*!
* @brief Create an ORTHOROMBIC UnitCell from the three lenghts
* @param a,b,c the three lenghts of the cell
* @return A pointer to the UnitCell
*/
CHFL_EXPORT CHFL_CELL* chfl_cell(double a, double b, double c);

/*!
* @brief Create a TRICLINIC UnitCell from the three lenghts and the three angles
* @param a,b,c the three lenghts of the cell
* @param alpha,beta,gamma the three angles of the cell
* @return A pointer to the UnitCell
*/
CHFL_EXPORT CHFL_CELL* chfl_cell_triclinic(double a, double b, double c, double alpha, double beta, double gamma);

/*!
* @brief Get the UnitCell from a frame
* @param frame the frame
* @return A pointer to the UnitCell
*/
CHFL_EXPORT CHFL_CELL* chfl_cell_from_frame(CHFL_FRAME* frame);

/*!
* @brief Get the cell volume.
* @param cell the unit cell to read
* @param V the volume
* @return The status code
*/
CHFL_EXPORT int chfl_cell_volume(const CHFL_CELL* cell, double* V);

/*!
* @brief Get the cell lenghts.
* @param cell the unit cell to read
* @param a,b,c the three cell lenghts
* @return The status code
*/
CHFL_EXPORT int chfl_cell_lengths(const CHFL_CELL* cell, double* a, double* b, double* c);

/*!
* @brief Set the unit cell lenghts.
* @param cell the unit cell to modify
* @param a,b,c the cell lenghts
* @return The status code
*/
CHFL_EXPORT int chfl_cell_set_lengths(CHFL_CELL* cell, double a, double b, double c);

/*!
* @brief Get the cell angles, in degrees.
* @param cell the cell to read
* @param alpha,beta,gamma the three cell angles
* @return The status code
*/
CHFL_EXPORT int chfl_cell_angles(const CHFL_CELL* cell, double* alpha, double* beta, double* gamma);

/*!
* @brief Set the cell angles, in degrees. This is only possible for TRICLINIC cells.
* @param cell the unit cell to modify
* @param alpha,beta,gamma the new angles values, in degrees
* @return The status code
*/
CHFL_EXPORT int chfl_cell_set_angles(CHFL_CELL* cell, double alpha, double beta, double gamma);

/*!
* @brief Get the unit cell matricial representation.
* @param cell the unit cell to use
* @param mat the matrix to fill. It should be a 3x3 matrix.
* @return The status code
*/
CHFL_EXPORT int chfl_cell_matrix(const CHFL_CELL* cell, double mat[3][3]);

//! Available cell types in chemfiles
typedef enum CHFL_CELL_TYPES {
    //! The three angles are 90°
    CHFL_CELL_ORTHOROMBIC = 0,
    //! The three angles may not be 90°
    CHFL_CELL_TRICLINIC = 1,
    //! Cell type when there is no periodic boundary conditions
    CHFL_CELL_INFINITE = 2,
} chfl_cell_type_t ;

/*!
* @brief Get the cell type
* @param cell the unit cell to read
* @param type the type of the cell
* @return The status code
*/
CHFL_EXPORT int chfl_cell_type(const CHFL_CELL* cell, chfl_cell_type_t* type);

/*!
* @brief Set the cell type
* @param cell the cell to modify
* @param type the new type of the cell
* @return The status code
*/
CHFL_EXPORT int chfl_cell_set_type(CHFL_CELL* cell, chfl_cell_type_t type);

/*!
* @brief Get the cell periodic boundary conditions along the three axis
* @param cell the cell to read
* @param x,y,z the periodicity of the cell along the three axis.
* @return The status code
*/
CHFL_EXPORT int chfl_cell_periodicity(const CHFL_CELL* cell, bool* x, bool* y, bool* z);

/*!
* @brief Set the cell periodic boundary conditions along the three axis
* @param cell the cell to modify
* @param x,y,z the new periodicity of the cell along the three axis.
* @return The status code
*/
CHFL_EXPORT int chfl_cell_set_periodicity(CHFL_CELL* cell, bool x, bool y, bool z);

/*!
* @brief Destroy an unit cell, and free the associated memory
* @param cell The cell to destroy
* @return The status code
*/
CHFL_EXPORT int chfl_cell_free(CHFL_CELL* cell);

/******************************************************************************/

/*!
* @brief Create a new empty topology
* @return A pointer to the new Topology
*/
CHFL_EXPORT CHFL_TOPOLOGY* chfl_topology();

/*!
* @brief Extract the topology from a frame
* @param frame The frame
* @return A pointer to the new Topology
*/
CHFL_EXPORT CHFL_TOPOLOGY* chfl_topology_from_frame(CHFL_FRAME* frame);

/*!
* @brief Get the current number of atoms in the topology.
* @param topology The topology to analyse
* @param natoms Will contain the number of atoms in the frame
* @return The status code
*/
CHFL_EXPORT int chfl_topology_atoms_count(const CHFL_TOPOLOGY* topology, size_t *natoms);

/*!
* @brief Add an atom at the end of a topology
* @param topology The topology
* @param atom The atom to be added
* @return The status code
*/
CHFL_EXPORT int chfl_topology_append(CHFL_TOPOLOGY* topology, const CHFL_ATOM* atom);

/*!
* @brief Remove an atom from a topology by index. This modify all the other atoms indexes.
* @param topology The topology
* @param i The atomic index
* @return The status code
*/
CHFL_EXPORT int chfl_topology_remove(CHFL_TOPOLOGY* topology, size_t i);

/*!
* @brief Tell if the atoms \c i and \c j are bonded together
* @param topology The topology
* @param i,j The atomic indexes
* @param result true if the atoms are bonded, false otherwise
* @return The status code
*/
CHFL_EXPORT int chfl_topology_isbond(const CHFL_TOPOLOGY* topology, size_t i, size_t j, bool* result);

/*!
* @brief Tell if the atoms \c i, \c j and \c k constitues an angle
* @param topology The topology
* @param i,j,k The atomic indexes
* @param result true if the atoms constitues an angle, false otherwise
* @return The status code
*/
CHFL_EXPORT int chfl_topology_isangle(const CHFL_TOPOLOGY* topology, size_t i, size_t j, size_t k, bool* result);

/*!
* @brief Tell if the atoms \c i, \c j, \c k and \c m constitues a dihedral angle
* @param topology The topology
* @param i,j,k,m The atomic indexes
* @param result true if the atoms constitues a dihedral angle, false otherwise
* @return The status code
*/
CHFL_EXPORT int chfl_topology_isdihedral(const CHFL_TOPOLOGY* topology, size_t i, size_t j, size_t k, size_t m, bool* result);

/*!
* @brief Get the number of bonds in the system
* @param topology The topology
* @param nbonds After the call, contains the number of bond
* @return The status code
*/
CHFL_EXPORT int chfl_topology_bonds_count(const CHFL_TOPOLOGY* topology, size_t* nbonds);

/*!
* @brief Get the number of angles in the system
* @param topology The topology
* @param nangles After the call, contains the number of angles
* @return The status code
*/
CHFL_EXPORT int chfl_topology_angles_count(const CHFL_TOPOLOGY* topology, size_t* nangles);

/*!
* @brief Get the number of dihedral angles in the system
* @param topology The topology
* @param ndihedrals After the call, contains the number of dihedral angles
* @return The status code
*/
CHFL_EXPORT int chfl_topology_dihedrals_count(const CHFL_TOPOLOGY* topology, size_t* ndihedrals);

/*!
* @brief Get the list of bonds in the system
* @param topology The topology
* @param data A nbonds x 2 array to be filled with the bonds in the system
* @param nbonds The size of the array. This should equal the value given by the
*               chfl_topology_bonds_count function
* @return The status code
*/
CHFL_EXPORT int chfl_topology_bonds(const CHFL_TOPOLOGY* topology, size_t (*data)[2], size_t nbonds);

/*!
* @brief Get the list of angles in the system
* @param topology The topology
* @param data A nangles x 3 array to be filled with the angles in the system
* @param nangles The size of the array. This should equal the value given by the
*               chfl_topology_angles_count function
* @return The status code
*/
CHFL_EXPORT int chfl_topology_angles(const CHFL_TOPOLOGY* topology, size_t (*data)[3], size_t nangles);

/*!
* @brief Get the list of dihedral angles in the system
* @param topology The topology
* @param data A ndihedrals x 4 array to be filled with the dihedral angles in the system
* @param ndihedrals The size of the array. This should equal the value given by the
*               chfl_topology_dihedrals_count function
* @return The status code
*/
CHFL_EXPORT int chfl_topology_dihedrals(const CHFL_TOPOLOGY* topology, size_t (*data)[4], size_t ndihedrals);

/*!
* @brief Add a bond between the atoms \c i and \c j in the system
* @param topology The topology
* @param i,j The atomic indexes
* @return The status code
*/
CHFL_EXPORT int chfl_topology_add_bond(CHFL_TOPOLOGY* topology, size_t i, size_t j);

/*!
* @brief Remove any existing bond between the atoms \c i and \c j in the system
* @param topology The topology
* @param i,j The atomic indexes
* @return The status code
*/
CHFL_EXPORT int chfl_topology_remove_bond(CHFL_TOPOLOGY* topology, size_t i, size_t j);

/*!
* @brief Destroy a topology, and free the associated memory
* @param topology The topology to destroy
* @return The status code
*/
CHFL_EXPORT int chfl_topology_free(CHFL_TOPOLOGY* topology);

/******************************************************************************/

/*!
* @brief Create an atom from an atomic name
* @param name The new atom name
* @return A pointer to the corresponding atom
*/
CHFL_EXPORT CHFL_ATOM* chfl_atom(const char* name);

/*!
* @brief Get a specific atom from a frame
* @param frame The frame
* @param idx The atom index in the frame
* @return A pointer to the corresponding atom
*/
CHFL_EXPORT CHFL_ATOM* chfl_atom_from_frame(const CHFL_FRAME* frame, size_t idx);

/*!
* @brief Get a specific atom from a topology
* @param topology The topology
* @param idx The atom index in the topology
* @return A pointer to the corresponding atom
*/
CHFL_EXPORT CHFL_ATOM* chfl_atom_from_topology(const CHFL_TOPOLOGY* topology, size_t idx);

/*!
* @brief Get the mass of an atom, in atomic mass units
* @param atom The atom
* @param mass The atom mass
* @return The status code
*/
CHFL_EXPORT int chfl_atom_mass(const CHFL_ATOM* atom, float* mass);

/*!
* @brief Set the mass of an atom, in atomic mass units
* @param atom The atom
* @param mass The new atom mass
* @return The status code
*/
CHFL_EXPORT int chfl_atom_set_mass(CHFL_ATOM* atom, float mass);

/*!
* @brief Get the charge of an atom, in number of the electron charge e
* @param atom The atom
* @param charge The atom charge
* @return The status code
*/
CHFL_EXPORT int chfl_atom_charge(const CHFL_ATOM* atom, float* charge);

/*!
* @brief Set the charge of an atom, in number of the electron charge e
* @param atom The atom
* @param charge The new atom charge
* @return The status code
*/
CHFL_EXPORT int chfl_atom_set_charge(CHFL_ATOM* atom, float charge);

/*!
* @brief Get the name of an atom
* @param atom The atom
* @param name A string buffer to be filled with the name
* @param buffsize The size of the string buffer
* @return The status code
*/
CHFL_EXPORT int chfl_atom_name(const CHFL_ATOM* atom, char* name, size_t buffsize);

/*!
* @brief Set the name of an atom
* @param atom The atom
* @param name A null terminated string containing the new name
* @return The status code
*/
CHFL_EXPORT int chfl_atom_set_name(CHFL_ATOM* atom, const char* name);

/*!
* @brief Try to get the full name of an atom from the short name
* @param atom The atom
* @param name A string buffer to be filled with the name
* @param buffsize The size of the string buffer
* @return The status code
*/
CHFL_EXPORT int chfl_atom_full_name(const CHFL_ATOM* atom, char* name, size_t buffsize);

/*!
* @brief Try to get the Van der Waals radius of an atom from the short name
* @param atom The atom
* @param radius The Van der Waals radius of the atom or -1 if no value could be found.
* @return The status code
*/
CHFL_EXPORT int chfl_atom_vdw_radius(const CHFL_ATOM* atom, double* radius);

/*!
* @brief Try to get the covalent radius of an atom from the short name
* @param atom The atom
* @param radius The covalent radius of the atom or -1 if no value could be found.
* @return The status code
*/
CHFL_EXPORT int chfl_atom_covalent_radius(const CHFL_ATOM* atom, double* radius);

/*!
* @brief Try to get the atomic number of an atom from the short name
* @param atom The atom
* @param number The atomic number, or -1 if no value could be found.
* @return The status code
*/
CHFL_EXPORT int chfl_atom_atomic_number(const CHFL_ATOM* atom, int* number);

//! Available types of atoms
typedef enum CHFL_ATOM_TYPES {
    //! Element from the periodic table of elements
    CHFL_ATOM_ELEMENT = 0,
    //! Corse-grained atom are composed of more than one element: CH3 groups,
    //! amino-acids are corse-grained atoms.
    CHFL_ATOM_CORSE_GRAIN = 1,
    //! Dummy site, with no physical reality
    CHFL_ATOM_DUMMY = 2,
    //! Undefined atom type
    CHFL_ATOM_UNDEFINED = 3,
} chfl_atom_type_t;

/*!
* @brief Get the atom type
* @param atom the atom to read
* @param type the type of the atom
* @return The status code
*/
CHFL_EXPORT int chfl_atom_type(const CHFL_ATOM* atom, chfl_atom_type_t* type);

/*!
* @brief Set the atom type
* @param atom the atom to modify
* @param type the new type of the atom
* @return The status code
*/
CHFL_EXPORT int chfl_atom_set_type(CHFL_ATOM* atom, chfl_atom_type_t type);

/*!
* @brief Destroy an atom, and free the associated memory
* @param atom The atom to destroy
* @return The status code
*/
CHFL_EXPORT int chfl_atom_free(CHFL_ATOM* atom);


#ifdef __cplusplus
}
#endif

#endif
