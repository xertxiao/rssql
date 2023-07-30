///*
// * Licensed to the Apache Software Foundation (ASF) under one
// * or more contributor license agreements.  See the NOTICE file
// * distributed with this work for additional information
// * regarding copyright ownership.  The ASF licenses this file
// * to you under the Apache License, Version 2.0 (the
// * "License"); you may not use this file except in compliance
// * with the License.  You may obtain a copy of the License at
// *
// *     http://www.apache.org/licenses/LICENSE-2.0
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the License is distributed on an "AS IS" BASIS,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the License for the specific language governing permissions and
// * limitations under the License.
// */
//
package tcExample.common.sink;

import org.apache.flink.annotation.PublicEvolving;
import org.apache.flink.streaming.api.functions.sink.SinkFunction;
//import org.apache.flink.walkthrough.common.entity.Alert;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import tcExample.common.entity.Arc;
import tcExample.common.entity.ArcSlide;

/** A sink for outputting alerts. */
@PublicEvolving
@SuppressWarnings("unused")
public class ArcSink implements SinkFunction<ArcSlide> {

    private static final long serialVersionUID = 1L;

    private static final Logger LOG = LoggerFactory.getLogger(ArcSink.class);

    @Override
    public void invoke(ArcSlide value, Context context) {
        LOG.info(value.toString());
    }
}