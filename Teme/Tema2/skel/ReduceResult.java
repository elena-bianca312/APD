/**
 * The type Reduce result.
 * Stores the result of ReduceFunction tasks.
 *
 */
public class ReduceResult {

    private String document;
    private float rang;
    private int maxLen;
    private int wordsWithMaxLen;

    /**
     * Instantiates a new Reduce result.
     *
     * @param document        the document
     * @param rang            the rang
     * @param maxLen          the max len
     * @param wordsWithMaxLen the words with max len
     */
    public ReduceResult(String document, float rang, int maxLen, int wordsWithMaxLen) {
        this.setDocument(document);
        this.setRang(rang);
        this.setMaxLen(maxLen);
        this.setWordsWithMaxLen(wordsWithMaxLen);
    }

    /**
     * Format output string.
     * Follows task criteria for output file write
     *
     * @return the string
     */
    String formatOutput() {
        return String.format("%s,%.2f,%d,%d\n",
                getDocument().substring(getDocument().lastIndexOf("/") + 1), getRang(), getMaxLen(),
                getWordsWithMaxLen());
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
     * Gets rang.
     *
     * @return the rang
     */
    public float getRang() {
        return rang;
    }

    /**
     * Sets rang.
     *
     * @param rang the rang
     */
    public void setRang(float rang) {
        this.rang = rang;
    }

    /**
     * Gets max len.
     *
     * @return the max len
     */
    public int getMaxLen() {
        return maxLen;
    }

    /**
     * Sets max len.
     *
     * @param maxLen the max len
     */
    public void setMaxLen(int maxLen) {
        this.maxLen = maxLen;
    }

    /**
     * Gets words with max len.
     *
     * @return the words with max len
     */
    public int getWordsWithMaxLen() {
        return wordsWithMaxLen;
    }

    /**
     * Sets words with max len.
     *
     * @param wordsWithMaxLen the words with max len
     */
    public void setWordsWithMaxLen(int wordsWithMaxLen) {
        this.wordsWithMaxLen = wordsWithMaxLen;
    }
}
