# unity3d学习笔记

## 1.基本概念

### 1.Scene

![](..\all_picture\u3d学习笔记\1.PNG)

**顶点的吸附**：

按v然后选择哪个点后，出现小白方框，往另一个物体的顶点上贴。

**物体轴心点**：只有在Scene面板有用，在检视面板没影响。

**物体轴向**：切换物体自身坐标和世界坐标的。

![](..\all_picture\u3d学习笔记\2.PNG)

**视图**：在Scene主界面点击切换。

![](..\all_picture\u3d学习笔记\3.PNG)



坐标系:

### 2.游戏对象：

![](..\all_picture\u3d学习笔记\4.PNG)

#### 1.组件：

![](..\all_picture\u3d学习笔记\5.PNG)

#### 2.材质：

![](..\all_picture\u3d学习笔记\6.PNG)

材质给有mesh renderer的物体才能直接拖过去赋予材质。

材质的渲染模式：

渲染模式总共有四种：

| 渲染模式    | 意思   | 适用对象举例 | 说明                                                         |
| ----------- | ------ | ------------ | ------------------------------------------------------------ |
| Opaque      | 不透明 | 石头         | 适用于所有的不透明的物体                                     |
| Cutout      | 镂空   | 破布         | *透明度不是\*0%\*就是\*100%\*，不存在半透明的区域。*****     |
| Fade        | 隐现   | 物体隐去     | *与\*Transparent\*的区别为高光反射会随着透明度而消失。***    |
| Transparent | 透明   | 玻璃         | 适用于像彩色玻璃一样的半透明物体，高光反射不会随透明而消失。 |

材质的属性：

![](..\all_picture\u3d学习笔记\7.PNG)

材质就是物体的质地，指色彩，纹理，光滑度，透明度，反射率，发光度等。实际就是Shader的实例。

Shader着色器： 专门用来渲染3D图形的技术，可以使纹理以某种方式展现。实际就是一段嵌入到渲染管线中的程序，可以控制GPU运算图像效果的算法。

Texture纹理：附加到物体表面的贴图。

![](..\all_picture\u3d学习笔记\8.PNG)



Shader定义了该材质可以设置什么功能，Material定义了给Shader的功能设置具体的值。











## 物理着色器:

基于光学物理公式创建的着色器.

![](..\all_picture\u3d学习笔记\1.jpg)



## 摄像机:

![](..\all_picture\u3d学习笔记\2.jpg)

clear flags: 处理屏幕的空白部分,(没物体的都叫空白部分).

1.Skybox(天空盒):是个材质,围绕整个场景的包装器,用于模拟天空的材质.   

使用方法:

![](..\all_picture\u3d学习笔记\3.jpg)

种类:    1.6sided : 6张图片拼成的天空盒.

​			2.procedural: 

2.Culling Mask(选择遮罩):

​	每一个游戏物体都有一个Layer层,和Tags标签.

​	而摄像机能选择看到哪些层的物体,或者标签的物体.

3.projection(投射):

​	正交模式:

​	perspective:  3d游戏一般的选择.

4.fileld of view:视野远近.

​	一般用来做狙击枪的镜.

5.clipping planes:剪裁面

最远能看多远,最近能看多近

​	far:设置哪些距离内的物体需要渲染.

​	near:  保持默认值, 从多远开始看.

6.viewport rect():

​	屏幕分屏, 可以用来做后视镜,小地图.

7.Depth(深度值):

​	摄像机深度值决定了渲染的层数,深度值越高显示越上边.



ctrl+shift+f: 将选中物体的transform值设置为目前所在位置的transform值.



## 基本概念

### 1.渲染管线

draw call(batches): 越大越消耗资源. 每帧调用显卡渲染物体的次数.

### 2.遮挡剔除

遮挡的部分不让渲染,节省资源.

使用一个插件进行遮挡剔除. [InstantOC]

### 3.多细节层次(LOD)

远距离时精细的模型会变模糊.

![](..\all_picture\u3d学习笔记\4.jpg)



## p22课:烘焙

烘焙:将阴影编译成变成贴图贴到场景中,节省性能,但没有实时光照,移动影子还是会在地上. 只针对不动的物体.

让光属性变成baked.

烘焙的步骤:

![](..\all_picture\u3d学习笔记\5.jpg)



## p23课: 背景音乐,场景音效.

## p24~p111课:都是c#程序教程

## p112~p148课: unity脚本教程

附加到游戏物体上的脚本类必须从MonoBehaviour类继承.

文件名与类名必须一致.

## p113: 修改脚本模板.

## p114:  脚本的生命周期.

特性:

SerializeField:序列化字段,在编辑器中显示私有变量. 

HideInInspector:在编译器中隐藏字段.

Range(0,100):让某个变量在0~100之间.



## p115:脚本的生命周期

![](..\all_picture\u3d学习笔记\6.jpg)

Awake:不管脚本是否启动,都会执行. 

FixdUpdate():每个固定时间执行一次,

![](..\all_picture\u3d学习笔记\7.jpg)

Update():每次渲染帧时执行一次. 执行间隔不固定.

## p116:输入事件

![](..\all_picture\u3d学习笔记\8.jpg)



## p120:常用API:Component

Component(组件):

Transform():

GameObject(游戏对象):

Object:

Time:

getComponent<name> 可以查找所有组件.



## p122: Transform

![](..\all_picture\u3d学习笔记\9.jpg)

![](..\all_picture\u3d学习笔记\11.jpg)

rotate:

![](..\all_picture\u3d学习笔记\12.jpg)

rotateAround:环绕旋转

```c#
this.transform.RotateAround(Vector3.zero, Vector3.up,1);//绕0点y轴绕1度.
//绕z轴:Vector3.forward
```



## p123:设置/获取父物体

```c#
//获取父物体变换组件
Transform parentTF = this.transform.parent;

//设置父父物体
public Transform tf;
this.transform.SetParent(tf);		//不写默认是true,视为世界坐标. 该物体位置不变
this.transform.SetParent(tf,false); //当前物体的位置,视为localPosition,该物体位置改变.
```



## p124:查找/设置子物体

```c#
//只能找子物体
Transform childTF = this.transorm.Find("子物体名称"); 

Transform childTF = this.transorm.Find("子物体名称/子物体名称"); 

//根据索引查找; 只能查找子物体
int count = this.transform.childCount;
for(int i=0;i<count; i++)
{
	Transform childTF = this.transfrom.GetChild(i);    
}

//所有对象解除父子关系
this.transform.DetachChildren();
//孩子解除父子关系
this.transform.SetParent(NULL);    

//一般不清楚层级时,使用递归查找,

```

Transform:查找(父,根,子(索引,名称))变换组件功能,改变位置,角度,大小功能.





## p125:gameObject、object

```c#
activeInHierarchy(); //游戏中的对象是否激活.
SetActive();//设置游戏启动/禁用

//任何组件都不能new好后直接挂到物体上,必须先用GameObject创建.
GameObject lightGo = new GameObject();
Light light = lightGO.AddComponent<Light>(); //给物体添加组件.  

//在场景中,根据名称查找物体(不建议使用)静态方法.
GameObject,Find("游戏对象名称");

//获取所有使用该标签的物体
GameObject[] allEnemy = GameObject.FindGameObjcetWithTag("Enemy");
//获取使用该标签的物体
GameObject playerGo = GameObject.FindWithTag("Player");
```

![](..\all_picture\u3d学习笔记\13.jpg)

Object:

```c#
name//游戏物体名称

Destroy//删除游戏对象,组件或资源    必须Destroy(this.gameObject)只用this不能删除

DestroyImmediate//立即删除游戏obj,慎用 .

DontDestroyOnLoad//加载新场景时,某些物体不被清除. 

//根据类型查找对象
Object.FindObjectOfType<MeshRenderer>(); //找一个.
```

## p126:常用类总结













































