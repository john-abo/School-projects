package ca.ucalgary.cpsc331;

public interface Dictionary {
    boolean empty();
    boolean full();
    void insert(String key);
    void delete(String key);
    boolean member(String key);
}
