
#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>


/**
* Implementation of the ServiceQueue ADT 
*/


class ServiceQueue {

  private:
    std::vector<int> the_queue;  // idx-0 is front of queue

    std::vector<int> buzzer_bucket;  // keeps track of buzzer-IDs
                                     //   that can be reused.  Operates
                                     //   as a stack so buzzer that became
                                     //   reusable most recently is on
                                     //   top (i.e., at back of vector).
 
  
   
  public:

	/**
	 * Function: snapshot()
         * param:  buzzers is an integer vector passed by ref
	 * Description:  populates buzzers vector with a "snapshot"
         *               of the queue as a sequence of buzzer IDs 
         *
	
	 */
	void snapshot(std::vector<int> &buzzers) {
             buzzers.clear();   // you don't know the history of the 
                                //   buzzers vector, so we had better
                                //   clear it first.

             // Note: the vector class over-rides the assignment operator '='
             //   and does an element-by-element copy from the RHS vector (the_vector)
             //   in this case) to the LHS vector (buzzers) in this case).

             buzzers = the_queue;

        }

	/**
	 * Function: length()
	 * Description:  returns the current number of
	 *    entries in the queue.
	 *
	 */
	int  length() {

             return the_queue.size();

        }

	/**
	 * Function: give_buzzer()
         * Return:   buzzer-ID (integer) assigned to the new customer.
	 * Description:  This is the "enqueue" operation.  For us
	 *    a "buzzer" is represented by an integer (starting
	 *    from zero).  The function selects an available buzzer 
	 *    and places a new entry at the end of the service queue 
	 *    with the selected buzer-ID. 
	 *    This buzzer ID is returned.
	 *    The assigned buzzer-ID is a non-negative integer 
	 *    with the following properties:
	 *
	 *       (1) the buzzer (really it's ID) is not currently 
	 *         taken -- i.e., not in the queue.  (It
	 *         may have been in the queue at some previous
	 *         time -- i.e., buzzer can be re-used).
	 *	  This makes sense:  you can't give the same
	 *	  buzzer to two people!
	 *
	 *       (2) Reusable Buzzers:  A re-usable buzzer is 
	 *	  a buzzer that _was_ in the queue at some previous
	 *	  time, but currently is not.
	 *
         *         REQUIREMENT:  If there is one or more reusable
         *         buzzer, you MUST return one of them; furthermore,
         *         it must be the buzzer that became reusable most 
         *         MOST RECENTLY.
	 *
	 *       (3) if there are no previously-used / reusable buzzers, 
         *         the SMALLEST possible buzzer-ID is used (retrieved from 
         *         inventory).  
	 *	    Properties in this situation (where N is the current
	 *	      queue length):
	 *
	 *		- The largest buzzer-ID used so far is N-1
	 *
	 *		- All buzzer-IDs in {0..N-1} are in the queue
	 *			(in some order).
	 *
	 *		- The next buzzer-ID (from the basement) is N.
	 *
	 *    In other words, you can always get more buzzers (from
	 *    the basement or something), but you don't fetch an
	 *    additional buzzer unless you have to (i.e., no reusable buzzers).
	 *
	 * Comments/Reminders:
	 *
	 *	Rule (3) implies that when we start from an empty queue,
	 *	the first buzzer-ID will be 0 (zero).
	 *
	 *	Rule (2) does NOT require that the _minimum_ reuseable 
	 *	buzzer-ID be used.  If there are multiple reuseable buzzers, 
	 *	any one of them will do.
	 *	
	 *	Note the following property:  if there are no re-useable 
	 *	buzzers, the queue contains all buzzers in {0..N-1} where
	 *       N is the current queue length (of course, the buzzer IDs 
	 *	may be in any order.)
	 *
	 *
	 */
	int  give_buzzer() {
          int b;
   
             // take top reusable buzzer if possible
             if(buzzer_bucket.size() != 0) {
                 b = buzzer_bucket.back();
                 buzzer_bucket.pop_back();
             }
             // otherwise, queue must contain exactly buzzers
             //    0..the_queue.size()-1
             // and therefore, the next smallest buzzer must be
             //    the_queue.size()
             else {
                 b = the_queue.size();
             }
             the_queue.push_back(b);
             return b;  
        }

	/**
	 * function: seat()
	 * description:  if the queue is non-empty, it removes the first 
	 *	 entry from (front of queue) and returns the 
	 *	 buzzer ID.
	 *	 Note that the returned buzzer can now be re-used.
	 *
	 *	 If the queue is empty (nobody to seat), -1 is returned to
	 *	 indicate this fact.
         *
         * Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	 *
	 */
	int seat() {
           int buzzer;

           if(the_queue.size()==0) 
              return -1;
           else {
              buzzer = the_queue[0];
              the_queue.erase(the_queue.begin(), the_queue.begin()+1);
              buzzer_bucket.push_back(buzzer);
              return buzzer;
           }
        }
     


	/**
	 * function: kick_out()
	 *
	 * description:  Some times buzzer holders cause trouble and
	 *		a bouncer needs to take back their buzzer and
	 *		tell them to get lost.
	 *
	 *		Specifially:
	 *
	 *		If the buzzer given by the 2nd parameter is 
	 *		in the queue, the buzzer is removed (and the
	 *		buzzer can now be re-used) and 1 (one) is
	 *		returned (indicating success).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 */
	bool kick_out(int buzzer) {
          std::vector<int>::iterator it;

             it = find(the_queue.begin(), the_queue.end(), buzzer);
             if(it == the_queue.end())
                 return false;
             else {
                 the_queue.erase(it);
                 buzzer_bucket.push_back(buzzer);
                 return true;
             }
        }

	/**
	 * function:  take_bribe()
	 * description:  some people just don't think the rules of everyday
	 *		life apply to them!  They always want to be at
	 *		the front of the line and don't mind bribing
	 *		a bouncer to get there.
	 *
	 *	        In terms of the function:
	 *
	 *		  - if the given buzzer is in the queue, it is 
	 *		    moved from its current position to the front
	 *		    of the queue.  1 is returned indicating success
	 *		    of the operation.
	 *		  - if the buzzer is not in the queue, the queue 
	 *		    is unchanged and 0 is returned (operation failed).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 */
	bool take_bribe(int buzzer) {
          std::vector<int>::iterator it;

             it = find(the_queue.begin(), the_queue.end(), buzzer);
             if(it == the_queue.end())
                 return false;
             else {
                 the_queue.erase(it);
                 the_queue.insert(the_queue.begin(), buzzer);
                 return true;
             }
         }

};   // end ServiceQueue class

#endif

