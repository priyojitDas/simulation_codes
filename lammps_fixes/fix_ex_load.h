/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

/* 
Some parts of this code are taken from https://github.com/polly-code/lammps_le
*/ 

#ifdef FIX_CLASS

FixStyle(ex_load,FixExLoad)

#else

#ifndef LMP_FIX_EX_LOAD_H
#define LMP_FIX_EX_LOAD_H

#include "fix.h"

namespace LAMMPS_NS {

class FixExLoad : public Fix {
 public:
  FixExLoad(class LAMMPS *, int, char **);
  ~FixExLoad();
  int setmask();
  void init();
  void init_list(int, class NeighList *);
  void pre_force(int);
  void post_integrate();
  void post_integrate_respa(int, int);

  int pack_forward_comm(int, int *, double *, int, int *);
  void unpack_forward_comm(int, int, double *);
  int pack_reverse_comm(int, int, double *);
  void unpack_reverse_comm(int, int *, double *);
  void grow_arrays(int);
  void copy_arrays(int, int, int);
  int pack_exchange(int, double *);
  int unpack_exchange(int, double *);
  double compute_vector(int);
  double memory_usage();

 private:
  int me;
  int iatomtype,jatomtype;
  int btype,seed;
  int imaxbond,jmaxbond;
  int inewtype,jnewtype;
  double cutsq,fraction;
  int atype,dtype,itype;
  int angleflag,dihedralflag,improperflag;
  int overflow;
  tagint lastcheck;

  int *bondcount;
  int createcount,createcounttotal;
  int nmax;
  tagint *partner,*finalpartner;
  double *distsq,*probability;

  int ncreate,maxcreate;
  tagint **created;

  tagint *copy;

  class RanMars *random;
  class NeighList *list;

  int countflag,commflag;
  int nlevels_respa;
  int nangles,ndihedrals,nimpropers;

  void check_ghosts();
  void update_topology();
  void rebuild_special_one(int);
  void create_angles(int);
  void create_dihedrals(int);
  void create_impropers(int);
  int dedup(int, int, tagint *);

  // DEBUG

  void print_bb();
  void print_copy(const char *, tagint, int, int, int, int *);
};

}

#endif
#endif

/* ERROR/WARNING messages:

E: Illegal ... command

Self-explanatory.  Check the input script syntax and compare to the
documentation for the command.  You can use -echo screen as a
command-line option when running LAMMPS to see the offending line.

E: Invalid atom type in fix ex_load command

Self-explanatory.

E: Invalid bond type in fix ex_load command

Self-explanatory.

E: Cannot use fix ex_load with non-molecular systems

Only systems with bonds that can be changed can be used.  Atom_style
template does not qualify.

E: Inconsistent iparam/jparam values in fix ex_load command

If itype and jtype are the same, then their maxbond and newtype
settings must also be the same.

E: Fix ex_load cutoff is longer than pairwise cutoff

This is not allowed because bond creation is done using the
pairwise neighbor list.

E: Fix ex_load angle type is invalid

Self-explanatory.

E: Fix ex_load dihedral type is invalid

Self-explanatory.

E: Fix ex_load improper type is invalid

Self-explanatory.

E: Cannot yet use fix ex_load with this improper style

This is a current restriction in LAMMPS.

E: Fix ex_load needs ghost atoms from further away

This is because the fix needs to walk bonds to a certain distance to
acquire needed info, The comm_modify cutoff command can be used to
extend the communication range.

E: New bond exceeded bonds per atom in fix ex_load

See the read_data command for info on setting the "extra bond per
atom" header value to allow for additional bonds to be formed.

E: New bond exceeded special list size in fix ex_load

See the special_bonds extra command for info on how to leave space in
the special bonds list to allow for additional bonds to be formed.

E: Fix ex_load induced too many angles/dihedrals/impropers per atom

See the read_data command for info on setting the "extra angle per
atom", etc header values to allow for additional angles, etc to be
formed.

E: Special list size exceeded in fix ex_load

See the read_data command for info on setting the "extra special per
atom" header value to allow for additional special values to be
stored.

W: Fix ex_load is used multiple times or with fix bond/break - may not work as expected

When using fix ex_load multiple times or in combination with
fix bond/break, the individual fix instances do not share information
about changes they made at the same time step and thus it may result
in unexpected behavior.

*/
