# Model Server Parameters {#ovms_docs_parameters}

### Configuration Parameters<a name="params"></a>:

<details><summary>Model configuration options</summary>

| Option  | Value format | Description | Required |
|---|---|---|---|
| `"model_name"/"name"` | `string` | model name exposed over gRPC and REST API.(use `model_name` in command line, `name` in json config)   | &check;|
| `"model_path"/"base_path"` | `"/opt/ml/models/model"`<br>"gs://bucket/models/model"<br>"s3://bucket/models/model"<br>"azure://bucket/models/model" | If using a Google Cloud Storage, Azure Storage or S3 path, see the requirements below.(use `model_path` in command line, `base_path` in json config)  | &check;|
| `"shape"` | `tuple, json or "auto"` | `shape` is optional and takes precedence over `batch_size`. The `shape` argument changes the model that is enabled in the model server to fit the parameters. <br><br>`shape` accepts three forms of the values:<br>* `auto` - The model server reloads the model with the shape that matches the input data matrix.<br>* a tuple, such as `(1,3,224,224)` - The tuple defines the shape to use for all incoming requests for models with a single input.<br>* A dictionary of shapes, such as `{"input1":"(1,3,224,224)","input2":"(1,3,50,50)", "input3":"auto"}` - This option defines the shape of every included input in the model.<br><br>Some models don't support the reshape operation.<br><br>If the model can't be reshaped, it remains in the original parameters and all requests with incompatible input format result in an error. See the logs for more information about specific errors.<br><br>Learn more about supported model graph layers including all limitations at [Shape Inference Document](https://docs.openvinotoolkit.org/2021.4/_docs_IE_DG_ShapeInference.html). ||
| `"batch_size"` | `integer / "auto"` | Optional. By default, the batch size is derived from the model, defined through the OpenVINO Model Optimizer. `batch_size` is useful for sequential inference requests of the same batch size.<br><br>Some models, such as object detection, don't work correctly with the `batch_size` parameter. With these models, the output's first dimension doesn't represent the batch size. You can set the batch size for these models by using network reshaping and setting the `shape` parameter appropriately.<br><br>The default option of using the Model Optimizer to determine the batch size uses the size of the first dimension in the first input for the size. For example, if the input shape is `(1, 3, 225, 225)`, the batch size is set to `1`. If you set `batch_size` to a numerical value, the model batch size is changed when the service starts.<br><br>`batch_size` also accepts a value of `auto`. If you use `auto`, then the served model batch size is set according to the incoming data at run time. The model is reloaded each time the input data changes the batch size. You might see a delayed response upon the first request.<br>  ||
| `"layout" `| `json / string` | `layout` is optional argument which allows to change the layout of model input and output tensors. Only `NCHW` and `NHWC` layouts are supported.<br><br>When specified with single string value - layout change is only applied to single model input. To change multiple model inputs or outputs, you can specify json object with mapping, such as: `{"input1":"NHWC","input2":"NHWC","output1":"NHWC"}`.<br><br>If not specified, layout is inherited from model. ||
| `"model_version_policy"` | `{ "all": {} }`<br>`{ "latest": { "num_versions":2 } }`<br>`{ "specific": { "versions":[1, 3] } }`</code> | Optional.<br><br>The model version policy lets you decide which versions of a model that the OpenVINO Model Server is to serve. By default, the server serves the latest version. One reason to use this argument is to control the server memory consumption.<br><br>The accepted format is in json.<br><br>Examples:<br><code>{"latest": { "num_versions":2 } # server will serve only two latest versions of model<br><br>{"specific": { "versions":[1, 3] } } # server will serve only versions 1 and 3 of given model<br><br>{"all": {} } # server will serve all available versions of given model ||
| `"plugin_config"` | json with plugin config mappings like`{"CPU_THROUGHPUT_STREAMS": "CPU_THROUGHPUT_AUTO"}` |  List of device plugin parameters. For full list refer to [OpenVINO documentation](https://docs.openvinotoolkit.org/2021.4/openvino_docs_IE_DG_supported_plugins_Supported_Devices.html) and [performance tuning guide](./performance_tuning.md)  ||
| `"nireq"` | `integer` | The size of internal request queue. When set to 0 or no value is set value is calculated automatically based on available resources.||
| `"target_device"` | `"CPU"/"HDDL"/"GPU"/"NCS"/"MULTI"/"HETERO"` | Device name to be used to execute inference operations. Refer to AI accelerators support below. ||
| `stateful` | `bool` | If set to true, model is loaded as stateful. ||
| `idle_sequence_cleanup` | `bool` | If set to true, model will be subject to periodic sequence cleaner scans. <br> See [idle sequence cleanup](stateful_models.md#stateful_cleanup). ||
| `max_sequence_number` | `uint32` | Determines how many sequences can be handled concurrently by a model instance. ||
| `low_latency_transformation` | `bool` | If set to true, model server will apply [low latency transformation](https://docs.openvinotoolkit.org/2021.4/openvino_docs_IE_DG_network_state_intro.html#lowlatency_transformation) on model load. ||

#### To know more about batch size, shape and layout parameters refer [Batch Size, Shape and Layout document](shape_batch_size_and_layout.md)

</details>


<details><summary>Server configuration options</summary>

Configuration options for server are defined only via command line options and determine configuration common for all served models. 

| Option  | Value format  | Description  | Required  |
|---|---|---|---|
| `port` | `integer` | Number of the port used by gRPC sever. | &check;|
| `rest_port` | `integer` | Number of the port used by HTTP server (if not provided or set to 0, HTTP server will not be launched). ||
| `grpc_bind_address` | `string` | Network interface address or a hostname, to which gRPC server will bind to. Default: all interfaces: 0.0.0.0 ||
| `rest_bind_address` | `string` | Network interface address or a hostname, to which REST server will bind to. Default: all interfaces: 0.0.0.0 ||
| `grpc_workers` | `integer` | Number of the gRPC server instances (must be from 1 to CPU core count). Default value is 1 and it's optimal for most use cases. Consider setting higher value while expecting heavy load. ||
| `rest_workers` | `integer` | Number of HTTP server threads. Effective when `rest_port` > 0. Default value is set based on the number of CPUs. ||
| `file_system_poll_wait_seconds` | `integer` | Time interval between config and model versions changes detection in seconds. Default value is 1. Zero value disables changes monitoring. ||
| `sequence_cleaner_poll_wait_minutes` | `integer` | Time interval (in minutes) between next sequence cleaner scans. Sequences of the models that are subjects to idle sequence cleanup that have been inactive since the last scan are removed. Zero value disables sequence cleaner.<br> See [idle sequence cleanup](stateful_models.md#stateful_cleanup). ||
| `cpu_extension` | `string` | Optional path to a library with [custom layers implementation](https://docs.openvinotoolkit.org/2021.4/openvino_docs_IE_DG_Extensibility_DG_Intro.html) (preview feature in OVMS).
| `log_level` | `"DEBUG"/"INFO"/"ERROR"` | Serving logging level ||
| `log_path` | `string` | Optional path to the log file. ||


</details>

