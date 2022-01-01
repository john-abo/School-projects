package ca.ucalgary.cpsc331;

public interface Dictionary {
    boolean empty();
    void insert(int key);
    void delete(int key);
    boolean member(int key);
}
