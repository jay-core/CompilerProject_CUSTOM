/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "parserDef.h"

#define EXCLUDE_EPS -288230376151711745L
#define INCLUDE_EPS 288230376151711744L

void first(Grammar g, rhsChar rcn, long int* firstBitString);

void computeFirst(Grammar g, nonTerminal nt, long int* firstBitString) {
    //this function assumes
    lhs lc = g[nt];
    for (int i=0; i<lc->numRules; i++) {
        first(lc->heads[i], firstBitString);
    }
}

void first(Grammar g, rhsChar rcn, long int* firstBitString) {
    // this funcyion assumes that the caller has already checked whether
    // the first set for nt has been previosly computed before calling this function
    if (rcn->tag==0) { //symbol is a terminal
        *firstBitString = *firstBitString | 1<<(rcn->s.t);
    } else { //symbol is a non-terminal => further digging needed
        lhs lc = g[rcn->s.nt];
        if (lc->first == 0) { //first set isn't initialized yet
            computeFirst(g, rcn->s.nt, &(lc->first));
        }
        
        if (lc->isNullable) {
            *firstBitString = *firstBitString | (lc->first & EXCLUDE_EPS); //exclude eps
            if (rcn->next != NULL)
                first(g, rcn->next, firstBitString);
            else
                //This was the last symbol in the rule
                *firstBitString = *firstBitString | INCLUDE_EPS;            
        } else {
            //no need to compute first sets of further nonTerminals in the rule
            *firstBitString = *firstBitString | (lc->first);
        }
    }
    return;
}

void computeFollow(Grammar g, nonTerminal nt) {
    // the function assumes that the caller has first checked if the follow set
    // of nt has already been computed
    rhsChar rcn, nextrcn;
    long int firstOfNext;
    for (int i=0; i<NO_OF_NONTERMINALS; i++) {
        for (int j=0; j<g[i].numRules; j++) {
            rcn = g[i].heads[j];
            while (rcn!=NULL) {
                if (rcn->tag==1 && rcn->s.nt==nt) {
                    if (rcn->next !=NULL) {
                        nextrcn = rcn->next;
                        first(g, nextrcn, &firstOfNext);
                        if ((firstOfNext>>eps) & 1 == 1) {
                            // first set of subsequent string contains eps
                            firstOfNext = firstOfNext & EXCLUDE_EPS;
                            g[nt].follow = g[nt].follow | firstOfNext;
                            if (i!=nt) // if the nonTerminal on LHS is different than nt
                            {
                                if (g[i].follow == 0)
                                    computeFollow(g, i);
                                g[nt].follow = g[nt].follow | g[i].follow;                             
                            }
                        } else {
                            g[nt].follow = g[nt].follow | firstOfNext;
                        }
                    } else {
                        //This is the last character in the RHS of the production
                        if (i!=nt) // if the nonTerminal on LHS is different than nt
                        {
                            if (g[i].follow == 0)
                                computeFollow(g, i);
                            g[nt].follow = g[nt].follow | g[i].follow;                             
                        }
                    }
                }
                rcn = rcn->next;
            }
        }
    }
}