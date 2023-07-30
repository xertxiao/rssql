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

package tcExample.common.entity;

import java.util.Objects;

/** A simple arc */
@SuppressWarnings("unused")
public final class Arc {

    private double fromId;

    private double toId;

//    private long iteration;

    private long timestamp;

    public Arc() {}


    public Arc(long fromId, long toId, long timestamp) {
        this.fromId = fromId;
        this.toId = toId;
        this.timestamp = timestamp;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public double getFromId() {
        return fromId;
    }

    public void setFromId(long fromId) {
        this.fromId = fromId;
    }

    public double getToId() {
        return toId;
    }

    public void setToId(long toId) {
        this.toId = toId;
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Arc arc = (Arc) o;
        return fromId == arc.fromId && toId == arc.toId && timestamp == arc.timestamp;

    }

    @Override
    public int hashCode() {
        return Objects.hash(fromId, toId, timestamp);
    }


    @Override
    public String toString() {
        return "Reachable {"
                + "fromId="
                + fromId
                + ", toId="
                + toId
                + ", ts="
                + timestamp
                + '}';
    }
}
