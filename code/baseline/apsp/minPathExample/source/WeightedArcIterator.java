/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package minPathExample.source;

//import org.apache.flink.walkthrough.common.entity.WeightedArc;

import minPathExample.entity.WeightedArc;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.sql.Timestamp;
import java.util.*;

/** An iterator of transaction events. */
final class WeightedArcIterator implements Iterator<WeightedArc>, Serializable {

//    private static final String FILE_PATH = "";

    private static final long serialVersionUID = 1L;

    private static final Timestamp INITIAL_TIMESTAMP = Timestamp.valueOf("2000-01-01 00:00:00");

    private static final long ONE_SEC = 1 * 1000;

    private final boolean bounded;

    private static Scanner input;

    private int mIndex = 0;

    private long timestamp;

//    private static List<WeightedArc> data = new ArrayList<>();

    static WeightedArcIterator bounded() {
        return new WeightedArcIterator(true);
    }

    static WeightedArcIterator unbounded() {
        return new WeightedArcIterator(false);
    }

//    private WeightedArc ParseLine(String line) {
//        String[] thisLine = line.split("\\s");
//        return new WeightedArc(Long.parseLong(thisLine[0]), Long.parseLong(thisLine[1]), 0L);
//    }

    private WeightedArc ParseLine(String line) {
        String[] thisLine = line.split("\\s");
        return new WeightedArc(Long.parseLong(thisLine[0]), Long.parseLong(thisLine[1]), Double.parseDouble(thisLine[2]), 0L);
    }

    private WeightedArcIterator(boolean bounded) {
        this.bounded = bounded;
        this.timestamp = INITIAL_TIMESTAMP.getTime();
        try {
            this.input = new Scanner(new File(""));
        } catch (IOException ie) {
            ie.printStackTrace();
        }
    }

    @Override
    public boolean hasNext() {

        return this.input.hasNext();
//        if (mIndex < data.size()) {
//            return true;
//        } else if (!bounded) {
//            mIndex = 0;
//            return true;
//        } else {
//            return false;
//        }
    }

    @Override
    public WeightedArc next() {
        Random random = new Random();
        double factor = 0 + random.nextDouble() * (3 - 0);
        WeightedArc arc = ParseLine(input.nextLine());
        timestamp += ONE_SEC * factor;
        arc.setTimestamp(timestamp);
        return arc;
    }

//    private static List<WeightedArc> data =
//            Arrays.asList(
//                    new WeightedArc(1, 3, 5, 0L),
//                    new WeightedArc(1, 2, 1, 0L),
//                    new WeightedArc(3, 5, 2, 0L),
//                    new WeightedArc(2, 3, 1, 0L),
//                    new WeightedArc(6, 7, 2, 0L));
}
