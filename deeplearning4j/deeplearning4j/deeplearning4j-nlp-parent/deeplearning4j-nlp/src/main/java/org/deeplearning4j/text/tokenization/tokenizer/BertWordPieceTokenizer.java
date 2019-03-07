/*******************************************************************************
 * Copyright (c) 2015-2019 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

package org.deeplearning4j.text.tokenization.tokenizer;

import lombok.extern.slf4j.Slf4j;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Pattern;

/**
 * A tokenizer that works with a vocab from a published bert model
 * @author Paul Dubs
 */
@Slf4j
public class BertWordPieceTokenizer implements Tokenizer {
    public static final Pattern splitPattern = Pattern.compile("(\\p{javaWhitespace}|((?<=\\p{Punct})|(?=\\p{Punct})))+");

    private final List<String> tokens;
    private TokenPreProcess tokenPreProcess;
    private AtomicInteger cursor = new AtomicInteger(0);

    public BertWordPieceTokenizer(String tokens, NavigableMap<String, Integer> vocab, boolean lowerCaseOnly) {
        if(vocab.comparator() == null || vocab.comparator().compare("a", "b") < 0){
            throw new IllegalArgumentException("Vocab must use reverse sort order!");
        }

        this.tokens = tokenize(vocab, tokens, lowerCaseOnly);
    }


    @Override
    public boolean hasMoreTokens() {
        return cursor.get() < tokens.size();
    }

    @Override
    public int countTokens() {
        return tokens.size();
    }

    @Override
    public String nextToken() {
        String base = tokens.get(cursor.getAndIncrement());
        if (tokenPreProcess != null)
            base = tokenPreProcess.preProcess(base);
        return base;
    }

    @Override
    public List<String> getTokens() {
        if (tokenPreProcess != null){
            final List<String> result = new ArrayList<>(tokens.size());
            for (String token : tokens) {
                result.add(tokenPreProcess.preProcess(token));
            }
            return result;
        }else {
            return tokens;
        }
    }

    @Override
    public void setTokenPreProcessor(TokenPreProcess tokenPreProcessor) {
        this.tokenPreProcess = tokenPreProcessor;

    }

    private List<String> tokenize(NavigableMap<String, Integer> vocab, String toTokenzie, boolean lowerCaseOnly) {
        final List<String> output = new ArrayList<>();

        String fullString = toTokenzie;
        if(lowerCaseOnly){
            fullString = fullString.toLowerCase();
        }

        for (String basicToken : splitPattern.split(fullString)) {
            String candidate = basicToken;

            while(candidate.length() > 0 && !"##".equals(candidate)){
                String longestSubstring = findLongestSubstring(vocab, candidate);
                output.add(longestSubstring);
                candidate = "##"+candidate.substring(longestSubstring.length());
            }
        }

        return output;
    }

    protected static String findLongestSubstring(NavigableMap<String, Integer> vocab, String candidate) {
        NavigableMap<String, Integer> tailMap = vocab.tailMap(candidate, true);
        String longestSubstring = tailMap.firstKey();
        int subStringLength = Math.min(candidate.length(), longestSubstring.length());
        while(!candidate.startsWith(longestSubstring)){
            subStringLength--;
            tailMap = tailMap.tailMap(candidate.substring(0, subStringLength), true);
            longestSubstring = tailMap.firstKey();
        }
        return longestSubstring;
    }

}
