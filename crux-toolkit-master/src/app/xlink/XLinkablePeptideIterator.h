/**
 * \file XLinkablePeptideIterator.h
 * AUTHOR: Sean McIlwain
 * CREATE DATE: 18 September December 2014
 * \brief Iterator for xlinkable peptides
 *****************************************************************************/

#ifndef XLINKABLEPEPTIDEITERATOR_H
#define XLINKABLEPEPTIDEITERATOR_H

#include "model/ModifiedPeptidesIterator.h"
#include "XLinkablePeptide.h"
#include "XLinkBondMap.h"

class XLinkablePeptideIterator {

 protected:
  ModifiedPeptidesIterator* peptide_iterator_; ///< peptide iterator to generate peptides
  XLinkablePeptide current_; ///< current xlinkable peptide
  XLinkBondMap bondmap_; ///< map of valid links
  FLOAT_T min_mass_;
  FLOAT_T max_mass_;
  bool has_next_; ///< is there a next candidate
  bool is_decoy_; ///< are we getting decoys
  std::vector<int> link_sites_; ///< list of links sites
  std::vector<XLinkablePeptide>::iterator iter_;
  std::vector<XLinkablePeptide>::iterator eiter_;

  /**
   * queues the next linkable peptide
   */
  void queueNextPeptide(); 

 public:

  /**
   * constructor that sets up the iterator
   */
  XLinkablePeptideIterator(
    double min_mass, ///< min mass of candidates
    double max_mass, ///< max mass of candidates
    Database* database, ///<peptide index
    PEPTIDE_MOD_T** peptide_mods, ///< current peptide mod
    int num_peptide_mods,
    bool is_decoy, ///< generate decoy candidates
    XLinkBondMap& bondmap ///< map of valid links
    );

  XLinkablePeptideIterator() {peptide_iterator_=NULL;}

  /**
   * Destructor
   */
  virtual ~XLinkablePeptideIterator();

  /**
   * \returns whether there is another linkable peptide
   */
  virtual bool hasNext();

  /**
   *\returns the next linkable peptide
   */
  virtual XLinkablePeptide& next();

};


#endif

/*                                                                                                                                                                                                                          
 * Local Variables:                                                                                                                                                                                                         
 * mode: c                                                                                                                                                                                                                  
 * c-basic-offset: 2                                                                                                                                                                                                        
 * End:                                                                                                                                                                                                                     
 */
