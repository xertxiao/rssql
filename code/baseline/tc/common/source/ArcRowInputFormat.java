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

import org.apache.flink.annotation.Internal;
import org.apache.flink.api.common.io.GenericInputFormat;
import org.apache.flink.api.common.io.NonParallelInput;
import org.apache.flink.core.io.GenericInputSplit;
import org.apache.flink.types.Row;
//import org.apache.flink.walkthrough.common.entity.WeightedArc;
import tcExample.common.entity.Arc;

import java.sql.Timestamp;
import java.util.Iterator;

/** An bounded input of transactions. */
@Internal
public class ArcRowInputFormat extends GenericInputFormat<Row> implements NonParallelInput {

    private static final long serialVersionUID = 1L;

    private transient Iterator<Arc> transactions;

    @Override
    public void open(GenericInputSplit split) {
        transactions = ArcIterator.bounded();
    }

    @Override
    public boolean reachedEnd() {
        return !transactions.hasNext();
    }

    @Override
    public Row nextRecord(Row reuse) {
        Arc arc = transactions.next();
        reuse.setField(0, arc.getFromId());
        reuse.setField(1, arc.getToId());
//        reuse.setField(2, arc.getIteration());
        reuse.setField(2, new Timestamp(arc.getTimestamp()));

        return reuse;
    }
}