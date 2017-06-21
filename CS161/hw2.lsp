;;
;;Jaehyung Park
;;CS 161 - HW2
;;504212821
;;

;;;;;;;;;;;;;;
; Question 1 ;
;;;;;;;;;;;;;;

; The BFS function takes a single argument FRINGE (list of search trees)
; and returns a top-level list of leaf nodes.
(defun BFS (FRINGE)
  (cond ; set initial condition
    ((null FRINGE) nil) ; when the FRINGE is null, return nil
    ; create a list with recursion of the first element of FRINGE and do another BFS
    ; for the rest of (cdr) FRINGE
    ((atom (car FRINGE)) (cons (car FRINGE) (BFS (cdr FRINGE))))
    (t (BFS (append (cdr FRINGE) (car FRINGE))))
    ))

;;;;;;;;;;;;;;
; Question 2 ;
;;;;;;;;;;;;;;


; These functions implement a depth-first solver for the homer-baby-dog-poison
; problem. In this implementation, a state is represented by a single list
; (homer baby dog poison), where each variable is T if the respective entity is
; on the west side of the river, and NIL if it is on the east side.
; Thus, the initial state for this problem is (NIL NIL NIL NIL) (everybody 
; is on the east side) and the goal state is (T T T T).

; The main entry point for this solver is the function DFS, which is called
; with (a) the state to search from and (b) the path to this state. It returns
; the complete path from the initial state to the goal state: this path is a
; list of intermediate problem states. The first element of the path is the
; initial state and the last element is the goal state. Each intermediate state
; is the state that results from applying the appropriate operator to the
; preceding state. If there is no solution, DFS returns NIL.
; To call DFS to solve the original problem, one would call 
; (DFS '(NIL NIL NIL NIL) NIL) 
; However, it should be possible to call DFS with a different initial
; state or with an initial path.

; First, we define the helper functions of DFS.

; FINAL-STATE takes a single argument S, the current state, and returns T if it
; is the goal state (T T T T) and NIL otherwise.
(defun FINAL-STATE (S)
  (cond ; set initial condition
    ((equal S '(T T T T))  T) ; take a single argument S and check if the goal state is equal to (T T T T)
                              ; if it's true, return T
    (t nil) ; if it's not equal to the goal state, return nil
    ))

; NEXT-STATE returns the state that results from applying an operator to the
; current state. It takes three arguments: the current state (S), and which entity
; to move (A, equal to h for homer only, b for homer with baby, d for homer 
; with dog, and p for homer with poison). 
; It returns a list containing the state that results from that move.
; If applying this operator results in an invalid state (because the dog and baby,
; or poisoin and baby are left unsupervised on one side of the river), or when the
; action is impossible (homer is not on the same side as the entity) it returns NIL.
; NOTE that next-state returns a list containing the successor state (which is
; itself a list); the return should look something like ((NIL NIL T T)).
(defun NEXT-STATE (S A) ; This is weird that the specs says it takes three arguments, but only shows S and A
                        ; which I used only 2 arguments
  (cond ; set initial condition
    ; First, we need to check the initial condition for illegal movements that returns nil
    ; illegal inputs should be all of these: both baby and poison, baby and dog without Homer
    ; return nil for impossible action
    ; any case with not (homer and baby) is illegal and baby with dog/poison is illegal
    ((and (not (equal (first S) (second S))) (equal (second S) (third S))) nil)
    ((and (not (equal (first S) (second S))) (equal (second S) (fourth S))) nil)
    ; below is more of a brute force method to find every single possible movement
    ; it seems to be easier to avoid the nil cases for the baby, since baby has strict illegal movements
    ; baby can't be with poison and can't be with dog without homer, which implies if homer is with something else
    ; than the baby, baby can't be with the other case (dog or poison)
    ; A equal to 'h (homer only)
    ((and (equal A 'h) 
          ; when A is equal to homer, any case that is not the above illegal move is okay
          (not (or (and (equal (first S) (second S)) (equal (second S) (third S)))
                   (and (equal (first S) (second S)) (equal (second S) (fourth S))))
          )) (list (list (not (first S)) (second S) (third S) (fourth S))))
    ; A equal to 'b (homer with baby)
    ((and (equal A 'b) 
          ; only with homer and baby
          (equal (first S) (second S))) (list (list (not (first S)) (not (second S)) (third S) (fourth S))
          ))
    ; A equal to 'd (homer with dog)
    ((and (equal A 'd) 
          ; when homer equal to dog and baby not equal to poison
          (and (equal (first S) (third S)) (not (equal (second S) (fourth S)))
          )) (list (list (not (first S)) (second S) (not (third S)) (fourth S))))
    ; A equal to 'p (homer and poison)
    ((and (equal A 'p) 
          ; homer equal to poison and baby not equal to dog (without homer)
          (and (equal (first S) (fourth S)) (not (equal (second S) (third S)))
          )) (list (list (not (first S)) (second S) (third S) (not (fourth S)))))
    ; all of these actions returns a list containing the successor state
    ))

; SUCC-FN returns all of the possible legal successor states to the current
; state. It takes a single argument (s), which encodes the current state, and
; returns a list of each state that can be reached by applying legal operators
; to the current state.
(defun SUCC-FN (S)
	; function SUCC-FN takes single argument S and takes that is passed to the NEXT-STATE function
	; that is to simply appended possible moves by each NEXT-STATE functions with a second argument 
  ; of the quotes of HOMER, BABY, DOG, and POISON
	; from NEXT-STATE description, h for homer only, b for homer with baby, d for homer with dog, and p for homer with poison
  ; any invalid movements will be ignored 
    (append (NEXT-STATE S 'h) (NEXT-STATE S 'b) (NEXT-STATE S 'd) (NEXT-STATE S 'p)
    ))

; ON-PATH checks whether the current state is on the stack of states visited by
; this depth-first search. It takes two arguments: the current state (S) and the
; stack of states visited by DFS (STATES). It returns T if s is a member of
; states and NIL otherwise.

; Take states as a list that the first element of the list can be removed
(defun ON-PATH (S STATES)
  (cond ; set initial condition
    ((null STATES) nil) ; return nil when no successor states to check
    ((equal s (car STATES)) T) ; if the current state (s) is the member of STATES, return T
    (t (ON-PATH S (cdr STATES))) ; take the rest of STATES to check the state through recursion until it finds it
    ))

; MULT-DFS is a helper function for DFS. It takes two arguments: a list of
; states from the initial state to the current state (PATH), and the legal
; successor states to the last, current state in the PATH (STATES). PATH is a
; first-in first-out list of states; that is, the first element is the initial
; state for the current search and the last element is the most recent state
; explored. MULT-DFS does a depth-first search on each element of STATES in
; turn. If any of those searches reaches the final state, MULT-DFS returns the
; complete path from the initial state to the goal state. Otherwise, it returns
; NIL.
; NIL.
(defun MULT-DFS (STATES PATH) ; takes two arguments PATH and STATES
  (cond ; set initial condition
    ((null STATES) nil) ; return nil when no successor states to check
                        ; PATH is a fifo list of states
    ((DFS (car STATES) PATH) (DFS (car STATES) PATH)) ; get the initial state and do DFS to reach to the goal state
    (t (MULT-DFS (cdr STATES) PATH))
    ; otherwise, do a recursive call of MULT-DFS with the remaining STATES
    ))
    ; otherwise, ignore this branch and call recursively mult-dfs on the rest states.

; DFS does a depth first search from a given state to the goal state. It
; takes two arguments: a state (S) and the path from the initial state to S
; (PATH). If S is the initial state in our search, PATH is set to NIL. DFS
; performs a depth-first search starting at the given state. It returns the path
; from the initial state to the goal state, if any, or NIL otherwise. DFS is
; responsible for checking if S is already the goal state, as well as for
; ensuring that the depth-first search does not revisit a node already on the
; search path.
(defun DFS (S PATH) ; takes two arguments S and PATH
  (cond ; set initial condition
    ; DFS is responsible for checking if S is already at the goal state (T T T T)
    ((FINAL-STATE S) (append PATH (list S)))
    ; if current state s has not been visited in path, we visit it, 
    ; running dfs on it by first calling mult-dfs
    ((not (ON-PATH S PATH)) (MULT-DFS (SUCC-FN S) (append PATH (list S))))
    ; ensure and check if S is already at the goal state, then there is no need to revisted the node on the search path
    (t nil)
    ))