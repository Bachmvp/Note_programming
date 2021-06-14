# Moveit gazebo controller 

用moveit联合gazebo做运动规划，主要的坑在控制器的设置

1.写机械臂的urdf文件，注意要加入传动装置（tranmission）,ros_control的插件，和joint_state_publisher的插件，注意设置命名空间为你的robot名字（保证在同一命名空间下）
2.用moveit setup assistant自动生成文件（load urdf文件）
3.然后我们需要编写启动gazebo的控制器生成文件了，一共有三个“控制文件”，但真正是控制器的只有一个，那就是joint trajectory controller,这个是接受规划好的轨迹点然后进行控制action。其他两个其实都是传递数据用的，第一个是follow joint tajectory 这个是用来打包好规划的点然后发出去的节点，还有一个joint state controller 是把传感器信息反馈给moveit.
4.那么launch文件都要干一些是什么
bringup.launch:
1.gazebo_world.launch:
1).把机器人urdf模型导入rosmaster
2)在gazebo里增生一个模型 通过gazebo_ros/spawn_model节点
2.joint_states.launch
1)导入joint_State的yaml文件（注意命名空间hello_ros_robot）(type:joint_state_controller/JointStateController)
2)增生一个joint_state)_controller,通过controller_manager/spawner节点（注意命名空间hello_ros_robot）
3)创建一个robot_state_publisher也就是robot的状态的发布器，因为我们是仿真，需要自己弄一个，通过joint_state_controller/robot_state_publisher节点 注意remap topic from="/joint_states" to="/hello_ros_robot/joint_states"
3.trajectory_controller.launch
1)导入trajectory controller的yaml文件（注意命名空间hello_ros_robot）(type: "position_controllers/JointTrajectoryController")  这里pid的具体数值可以写可不写
2）增生一个trajectory_controller,通过controller_manager/spawner节点（注意命名空间hello_ros_robot）
4.hello_ros_moveit_confiig/launch/move_group.launch(moveit自动生成的)（在这个启动文件里会启动一个子文件）
1）hello_ros_robot_moveit_controller_manager.launch.xml 这个启动文件主要是导入follow_joint_trajectory的yaml信息的（controller_manager_ns: controller_manager）（name: hello_ros_robot/arm_controller）
5.hello_ros_moveit_confiig)/launch/moveit_rviz.launch(注意arg name="rviz_config" value="true")（自动生成的）
6.最后很重要的一点，我们必须创建一个joint_state_publisher，我们没有真的机器人，必须发布一个假的joint state信息才可以 通过joint_state_controller/joint_state_publisher（<param name="/use_gui" value="false"/>）（<rosparam param="/source_list">[/hello_ros_robot/joint_states]</rosparam>）


那么这就是启动文件的全过程
在debug的过程中
1。找不到controller的类型，原来是系统没有装，这几个控制器必须一个一个单独装才可以
$ sudo apt-get install ros-indigo-joint-state-controller : This will install joint_state_controller package

$ sudo apt-get install ros-indigo-effort-controllers : This will install Effort controller

$ sudo apt-get install ros-indigo-position-controllers : This will install position controllers

2。rospy出现了问题，也是少装了一个东西