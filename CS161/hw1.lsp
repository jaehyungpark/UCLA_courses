;;
;;Jaehyung Park
;;CS 161 - HW1
;;504212821
;;

;; Question 1
;; given PAD(n+1) = PAD(n-1) + PAD(n-2)
;; with PAD(0) = PAD(1) = PAD(2) = 1
;; that takes a single integer argument N and returns the Nth padovan number
(defun PAD (N)
  (cond	; set initial condition
  	((= N 0) 1)	; if N is 0, return 1
  	((= N 1) 1)	; if N is 1, return 1
  	((= N 2) 1)	; if N is 2, return 1
  	(t(+ (PAD (- N 2)) (PAD (- N 3))))
    ))

;; Question 2
;; given SUMS(N+1) = SUMS(N-1) + SUMS(N-2)
;; set with SUMS(0) = SUMS(1) = SUMS(2) = 0
;; same as PAD, it takes a single numeric argument N and returns the number of additions
;; required by the PAD function to compute the Nth padovan number.
(defun SUMS (N)
  (cond ; set initial condition
  	((= N 0) 0) ; if N is 0, return 0
  	((= N 1) 0) ; if N is 1, return 0
  	((= N 2) 0) ; if N is 2, return 0
  	(t(+ 1 (SUMS (- N 2)) (SUMS (- N 3))))
    ))

;; Question 3
;; the basic idea is that ANON(tree) = (ANON(left_tree) ANON(right_tree))
;; that the function ANON takes a TREE (represents a tree) as an argument and returns a 
;; anonymized tree structure where all the symbols and numbers in the tree are replace
;; by a ? mark.
(defun ANON (TREE) ; takes a tree as an argument
 (cond ; set initial condtion
  ((null TREE) nil) ; when the tree is null, return nil
  ((atom TREE) '?)  ; when the tree reaches to the atom, return '? (quote of ?)
  (t(cons (ANON (car TREE)) (ANON (cdr TREE))))
  ))
