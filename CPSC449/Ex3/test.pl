quicksort([],[]).
quicksort([X|Xs],Ys) :-
    partition(Xs,X,Left,Right),
    quicksort(Left,Ls),
    quicksort(Right,Rs),
    append(Ls,[X|Rs],Ys).

partition([],Y,[],[]).
partition([X|Xs],Y,[X|Ls],Rs) :-
    X < Y,
    partition(Xs,Y,Ls,Rs).
partition([X|Xs],Y,Ls,[X|Rs]) :-
    X >= Y,
    partition(Xs,Y,Ls,Rs).

append([],Ys,Ys).
append([X|Xs],Ys,[X|Zs]) :- append(Xs,Ys,Zs).