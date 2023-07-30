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

package minPathExample.entity;

import java.util.Objects;

/** A simple arc */
@SuppressWarnings("unused")
public final class WeightedArc {

    private long fromId;

    private long toId;

    private double weight;

    private long timestamp;

    public WeightedArc() {}

    public WeightedArc(long fromId, long toId, double weight, long timestamp) {
        this.fromId = fromId;
        this.toId = toId;
        this.weight = weight;
        this.timestamp = timestamp;
    }

    public long getFromId() {
        return fromId;
    }

    public void setFromId(long fromId) {
        this.fromId = fromId;
    }

    public long getToId() {
        return toId;
    }

    public void setToId(long toId) {
        this.toId = toId;
    }

    public double getWeight() {
        return weight;
    }

    public void setWeight(double weight) {
        this.weight = weight;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof WeightedArc)) return false;
        WeightedArc that = (WeightedArc) o;
        return fromId == that.fromId && toId == that.toId && Double.compare(that.weight, weight) == 0 && timestamp == that.timestamp;
    }

    @Override
    public int hashCode() {
        return Objects.hash(fromId, toId, weight, timestamp);
    }

    @Override
    public String toString() {
        return "WeightedArc{" +
                "fromId=" + fromId +
                ", toId=" + toId +
                ", weight=" + weight +
                ", timestamp=" + timestamp +
                '}';
    }
}
