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

package tcExample.common.source;

//import org.apache.flink.walkthrough.common.entity.WeightedArc;
import tcExample.common.entity.Arc;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.Timestamp;
import java.util.*;
import java.util.stream.Stream;

/** An iterator of transaction events. */
final class ArcIterator implements Iterator<Arc>, Serializable {

    private static final String FILE_PATH = "/home/ubuntu/flink-project/dataset/lumbTc.data";

    private static final long serialVersionUID = 1L;

    private static final Timestamp INITIAL_TIMESTAMP = Timestamp.valueOf("2000-01-01 00:00:00");

    private static final long ONE_SEC = 1 * 1000;

    private final boolean bounded;

    private int mIndex = 0;

    private long timestamp;

    private static Scanner input;


    static ArcIterator bounded() {
        return new ArcIterator(true);
    }

    static ArcIterator unbounded() {
        return new ArcIterator(false);
    }

    private Arc ParseLine(String line) {
        String[] thisLine = line.split("\\s");
        return new Arc(Long.parseLong(thisLine[0]), Long.parseLong(thisLine[1]), 0L);
    }

    private ArcIterator(boolean bounded) {
        this.bounded = bounded;
        this.timestamp = INITIAL_TIMESTAMP.getTime();
        try {
            this.input = new Scanner(new File(FILE_PATH));
        } catch (IOException ie) {
            ie.printStackTrace();
        }

    }

    @Override
    public boolean hasNext() {
        return input.hasNext();

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
    public Arc next() {
        //Arc arc = data.get(mIndex++);
        Random random = new Random();
        double factor = 0 + random.nextDouble() * (3 - 0);
        Arc arc = ParseLine(input.nextLine());
        timestamp += ONE_SEC ;
        arc.setTimestamp(timestamp);
        return arc;
    }

//    private static List<Arc> data =
//            Arrays.asList(
//                    new Arc(1, 2, 0L),
//                    new Arc(2, 3, 0L),
//                    new Arc(3, 4, 0L),
//                    new Arc(4, 5, 0L),
//                    new Arc(6, 7, 0L));
}
