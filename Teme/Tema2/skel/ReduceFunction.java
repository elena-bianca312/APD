import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * The type Reduce function.
 * Implements Callable instead of Runnable so we can return and process
 * the results of the tasks
 */
public class ReduceFunction implements Callable<ReduceResult> {

    private String document;
    private List<MapResult> mapResults;

    private Map<Integer, Integer> map = new HashMap<>();
    private List<String> maxLenWords = new ArrayList<>();

    /**
     * Instantiates a new Reduce function.
     *
     * @param document   the document
     * @param mapResults the map results
     */
    public ReduceFunction(String document, List<MapResult> mapResults) {
        this.setDocument(document);
        this.setMapResults(mapResults);
    }

    @Override
    public ReduceResult call() throws Exception {

        // Compute final dictionary
        // Merge the lists with the words of maximum length into one -> maxLenWords
        for (MapResult mapResult : getMapResults()) {
            setMap(Stream.concat(getMap().entrySet().stream(),
                    mapResult.getDictionary().entrySet().stream()).collect(
                    Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, Integer::sum)));
            getMaxLenWords().addAll(mapResult.getMaxLengthWords());
        }

        // Compute (process) list with max len words
        getMaxLenWords().sort((o1, o2) -> o2.length() - o1.length());
        int maxLen = getMaxLenWords().get(0).length();
        getMaxLenWords().removeIf(element -> element.length() < maxLen);

        // Compute file rank by given formula, using Fibonacci sequence
        AtomicInteger fibSum = new AtomicInteger();
        getMap().forEach((k, v) -> fibSum.addAndGet(fib(k + 1) * v));
        float sum = fibSum.floatValue();

        // Compute max word length in file and total number of words in file
        int maxValueInMap = (Collections.max(getMap().keySet()));
        float totalWords = getMap().values().stream().mapToInt(Integer::intValue).sum();

        return new ReduceResult(getDocument(), sum / totalWords, maxValueInMap,
                getMap().get(maxValueInMap));
    }

    /**
     * Fib int.
     *
     * @param n the n
     * @return the int
     */
    int fib(int n) {
        if (n <= 1) {
            return n;
        }
        return fib(n - 1) + fib(n - 2);
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
     * Gets map results.
     *
     * @return the map results
     */
    public List<MapResult> getMapResults() {
        return mapResults;
    }

    /**
     * Sets map results.
     *
     * @param mapResults the map results
     */
    public void setMapResults(List<MapResult> mapResults) {
        this.mapResults = mapResults;
    }

    /**
     * Gets map.
     *
     * @return the map
     */
    public Map<Integer, Integer> getMap() {
        return map;
    }

    /**
     * Sets map.
     *
     * @param map the map
     */
    public void setMap(Map<Integer, Integer> map) {
        this.map = map;
    }

    /**
     * Gets max len words.
     *
     * @return the max len words
     */
    public List<String> getMaxLenWords() {
        return maxLenWords;
    }

    /**
     * Sets max len words.
     *
     * @param maxLenWords the max len words
     */
    public void setMaxLenWords(List<String> maxLenWords) {
        this.maxLenWords = maxLenWords;
    }
}
