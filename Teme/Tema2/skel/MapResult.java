import java.util.List;
import java.util.Map;

/**
 * The type Map result.
 * Stores the result of MapFunction tasks.
 */
public class MapResult {

    private String document;
    private Map<Integer, Integer> dictionary;
    private List<String> maxLengthWords;

    /**
     * Instantiates a new Map result.
     *
     * @param document       the document
     * @param dictionary     the dictionary
     * @param maxLengthWords the max length words
     */
    public MapResult(String document, Map<Integer, Integer> dictionary,
                     List<String> maxLengthWords) {
        this.setDocument(document);
        this.setDictionary(dictionary);
        this.setMaxLengthWords(maxLengthWords);
    }

    /**
     * Gets document.
     *
     * @return the document
     */
    public String getDocument() {
        return document;
    }

    /**
     * Sets document.
     *
     * @param document the document
     */
    public void setDocument(String document) {
        this.document = document;
    }

    /**
     * Gets dictionary.
     *
     * @return the dictionary
     */
    public Map<Integer, Integer> getDictionary() {
        return dictionary;
    }

    /**
     * Sets dictionary.
     *
     * @param dictionary the dictionary
     */
    public void setDictionary(Map<Integer, Integer> dictionary) {
        this.dictionary = dictionary;
    }

    /**
     * Gets max length words.
     *
     * @return the max length words
     */
    public List<String> getMaxLengthWords() {
        return maxLengthWords;
    }

    /**
     * Sets max length words.
     *
     * @param maxLengthWords the max length words
     */
    public void setMaxLengthWords(List<String> maxLengthWords) {
        this.maxLengthWords = maxLengthWords;
    }
}
