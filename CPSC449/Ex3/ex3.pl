% Instructions:
%   - Please submit ONLY this file up to GradeScope!
%   - Implement as many of the predicates as you can as specified in the assignment!
%   - Have lots of fun! Wahoo! Pain and suffering
% Prolog executable cannot lint
%   - Do not change the module information of this file or the name of this file (i.e., do not change line 6)!
:- module( ex3, [ myappend/3 , myreverse/2 , myflatten/2 , mymember/2 , myremove/3 , mymember2/2 , mysubstring/2 , mysublists/2 , mypermutation/2 , grandfather/2 , grandmother/2 , brother/2 , sister/2 , sibling/2 , cousin/2 , contact/3 , josephus/5 ]).


%%%% Question 1.

% My append was given in the lectures
myappend([],X,X).           % This appears to be some so
myappend([H|T],X,[H|S]) :-
    myappend(T,X,S).

% My reverse was also given in the lectures
% This, and the next myreverse, all seem to timeout
myreverse(X,Y) :-
    shunt(X,[],Y).
shunt([],Y,Y).
shunt([H|T],P,X) :-
    shunt(T,[H|P],X).   % Nothing new, usual shunt behaviour to move the head from list to another

% NEW myreverse, which is actually the original slow version lol. NVM, this thing also times out
%myreverse([],[]).
%myreverse([H|T],Y) :-
%    myreverse(T,W),
%    myappend(W,[H],Y).


% What did this even mean again?
% Ok i remember now
myflatten([],[]).
myflatten([H|T],X) :-
    myappend(H,Z,X),    % Append the first list in the list of lists to Z
    myflatten(T,Z).     % Z is the result of the next flatten

% I know this means searching for the member in a list
mymember(S, [S|_T]).   % The head is the same as the element being searched
mymember(S, [_H|X]) :-      % Search the rest of the list
    mymember(S,X).

% But this tho
myremove(X, [X|T], T).          % So if X is found in Y, just give T?
myremove(X, [Y|T], [Y|R]) :-  % I think this works as an "otherwise??
    myremove(X,T,R).

% idk if this works, but lets hope it does
mymember2sub(S, [S|T]) :-
    not(mymember(S,T)).      % Now this needs to make sure there isn't any occurance of s in [S|_T]
mymember2sub(S, [_H|X]) :-  % Search the rest of the list
    mymember2sub(S,X).

%%%% Question 2.
mymember2(S, [S|T]) :-  % One member has been found
    mymember2sub(S,T).      % Then you just need to look for one more member. Nvm, it needs to occur strictly twice
mymember2(S,[_H|X]) :-
    mymember2(S,X).     % Otherwise keep searching

%%%% Question 3. Substring must exist as a sequence in given list
mysubstring([], _).
mysubstring(X,Y) :-
    subcheck(X,X,Y).

% Honestly just going to copy what I did for exercise 1, and just try to convert
subcheck(_,[],_).
subcheck(A, [Ha|Ta], [Ha|Tb]) :-        % If the first characters match, keep going I guess
    subcheck(A,Ta,Tb).
subcheck(A, [_Ha|_Ta], [_Hb|Tb]) :-     % and if different I guess?
    subcheck(A, A, Tb).                 % Holy shit it worked


%%%% Question 4. Sublist doesn't need to exist as a sequence
% ight nvm, I totally misread the instructions on this. This wants to
% generate all sublists of S, mysublists(X,S).
mysublists([],[[]]).    %  There is nothing left. Only ruin
mysublists([H|T],X) :-
    myappend(A,B,X),
    mysublists(T,A),    % sublists without the head
    slaphead(H,A,B),    % This function just 'slaps' H to the head of all lists A
    !.                  % I forgot this piece of excrement and it was killing me for the longest time

slaphead(_H,[],[]).
slaphead(H,[A|As],[[H|A]|Bs]) :-
    slaphead(H,As,Bs).

%%%% Question 5
% it seems prolog just sorts the given lists and compares them that way?
% Nvm, it seems all these tests actually just want to generate the permutations
mypermutation([],[]).
mypermutation(A,[B|Bs]) :-
    mydelete(B,A,As),       % Delete remove B from the original list A
    mypermutation(As,Bs).   % If the sublists generated are permutations, then the lists are permutations

% Just gotta write delete real quick
mydelete(X,[X|T],T).            % Where the item being removed is the head, then it is deleted
mydelete(X,[NX|T1],[NX|T2]) :-  % Where the heads are the same (making the lists still equal), but not X,
    mydelete(X,T1,T2).          % Then just keep looking

% I spent so much time working on this solution only to realize it wasn't what was wanted :')
% in the tests
%    mymember(H,B),
%    mypermutation(T,B).     % Just brute force until I can figure out this sort shenanigans
%    %!.                      % LOL this is taking too long to pass
%
% Sorting has failed me. It can't find all permutations
%    mysort(A,Sorted),    % Sort A
%    mysort(B,Sorted).    % Sort B, if they're both equal to Sorted, then they're equal
%
% Yeah, definitely going to need to use quick sort
%mysort([],[]).
%mysort([P|T],Sorted) :-
%    mypartition(T,P,Left,Right),      % Partition the lists
%    mysort(Left,A),                % Sort left side
%    mysort(Right,B),               % Sort right side
%    myappend(A,[P|B],Sorted).     % Recombine lists
%
%mypartition([],_P,[],[]).                % Nothing left to partition. I think this is the bastard that's failing me
%mypartition([H|T],P,Left,[H|Right]) :-       % H >= P, [H|right]
%    H >= P,
%    mypartition(T,P,Left,Right).
%mypartition([H|T],P,[H|Left],Right) :-        % H < P, [H|left]
%    H < P,
%    mypartition(T,P,Left,Right).
    
%%%% Question 6

% Understand these predicates as follows.
%   son(Mom, Dad, Child)      is read as ``Child is the son of the mother Mom and the father Dad''
%   daughter(Mom, Dad, Child) is read as ``Child is the daughter of the mother Mom and the father Dad''

son(mymom, mydad, theson).
daughter(mymom, mydad, thedaughter).
% Add your own family members too!

% Understand these predicates as follows.
%   grandfather(A,B). is read as ``A is a grandfather of B''
%   grandmother(A,B). is read as ``A is a grandmother of B''
%   brother(A,B).     is read as ``A is a brother of B''
%   sister(A,B).      is read as ``A is a sister of B''
%   sibling(A,B).     is read as ``A is a sibling of B''
%   cousin(A,B).      is read as ``A is a cousin of B''

% Grandfather shenanigans
% Grand F -> F -> Son
grandfather(A,B) :-
    son(_,A,C),
    son(_,C,B).
% Grand F -> M -> Son  
grandfather(A,B) :-
    daughter(_,A,C),
    son(C,_,B).
% Grand F -> F -> Daughter
grandfather(A,B) :-
    son(_,A,C),
    daughter(_,C,B).
% Grand F -> M -> Daughter  
grandfather(A,B) :-
    daughter(_,A,C),
    daughter(C,_,B).

% Grandmother shenanigans
% Grand M -> F -> Son
grandmother(A,B) :-
    son(A,_,C),
    son(_,C,B).
% Grand M -> M -> Son  
grandmother(A,B) :-
    daughter(A,_,C),
    son(C,_,B).
% Grand M -> F -> Daughter
grandmother(A,B) :-
    son(A,_,C),
    daughter(_,C,B).
% Grand M -> M -> Daughter  
grandmother(A,B) :-
    daughter(A,_,C),
    daughter(C,_,B).

% Brother Shenanigans
brother(A,B) :-
    son(X,Y,A),
    daughter(X,Y,B).

brother(A,B) :-
    son(X,Y,A),
    son(X,Y,B).

% Sister Shenanigans
sister(A,B) :-
    daughter(X,Y,A),
    son(X,Y,B).
sister(A,B) :-
    daughter(X,Y,A),
    daughter(X,Y,B).

% Sibling Shenanigans
sibling(A,B) :-
    sister(A,B).
sibling(A,B) :-
    brother(A,B). 

% Cousin Shenanigans
%  Male cousin, dad's side, Male
cousin(A,B) :- A\=B, son(X,_,A), sibling(X,Z), son(_,Z,B).
cousin(A,B) :- A\=B, son(_,Y,A), sibling(Y,Z), son(_,Z,B).

%  Female cousin, dad's side, Female
cousin(A,B) :- A\=B, daughter(X,_,A), sibling(X,Z), daughter(_,Z,B).
cousin(A,B) :- A\=B, daughter(_,Y,A), sibling(Y,Z), daughter(_,Z,B).

%  Male cousin, mom's side, Male
cousin(A,B) :- A\=B, son(X,_,A), sibling(X,Z), son(Z,_,B).
cousin(A,B) :- A\=B, son(_,Y,A), sibling(Y,Z), son(Z,_,B).

%  Female cousin, mom's side, Female
cousin(A,B) :- A\=B, daughter(X,_,A), sibling(X,Z), daughter(Z,_,B).
cousin(A,B) :- A\=B, daughter(_,Y,A), sibling(Y,Z), daughter(Z,_,B).

% It just keeps on going
%  Male cousin, dad's side, Female
cousin(A,B) :- A\=B, son(X,_,A), sibling(X,Z), daughter(_,Z,B).
cousin(A,B) :- A\=B, son(_,Y,A), sibling(Y,Z), daughter(_,Z,B).

%  Female cousin, dad's side, Male
cousin(A,B) :- A\=B, daughter(X,_,A), sibling(X,Z), son(_,Z,B).
cousin(A,B) :- A\=B, daughter(_,Y,A), sibling(Y,Z), son(_,Z,B).

%  Male cousin, mom's side, Female
cousin(A,B) :- A\=B, son(X,_,A), sibling(X,Z), daughter(Z,_,B).
cousin(A,B) :- A\=B, son(_,Y,A), sibling(Y,Z), daughter(Z,_,B).

%  Female cousin, mom's side, Male
cousin(A,B) :- A\=B, daughter(X,_,A), sibling(X,Z), son(Z,_,B).
cousin(A,B) :- A\=B, daughter(_,Y,A), sibling(Y,Z), son(Z,_,B).

% Bruh wtf, I'm only failing one test???? but where??????????
% Wait, I think I found it
% NOPE :')
% Looking at the test, apparently A is B, so it's like they're giving cousing(A,A).
% Man, this database stuff sucks lols

%%%% Question 7
contact(_,_,_).

%%%% Question 8
% Please see the assignment for the logic puzzle. 
% This question will just be hand graded!

%%%% Question 9
% The parameters are as follows...
%   - NumberOfSoldiers: the total number of soldiers including Josephus and his accomplice (> 2)
%   - StartingPosition: the starting position
%   - N: The selected number to count down
%   - J: Output position for Josephus (< NumberOfSoldiers)
%   - A: Output position for the accomplice (< NumberOfSoldiers)
% where all positions are 0 indexed
josephus(NumberOfSoldiers, StartingPosition, N, J, A).
