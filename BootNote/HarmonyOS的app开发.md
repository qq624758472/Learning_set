# 鸿蒙app开发总结

鸿蒙官方开发文档:https://developer.harmonyos.com/cn/docs/documentation/doc-guides/ability-page-lifecycle-0000000000029840

## 1.鸿蒙app开发环境搭建



## 2.Ability

 Ability是应用所具备能力的抽象，也是应用程序的重要组成部分。一个应用可以具备多种能力（即可以包含多个Ability），HarmonyOS支持应用以Ability为单位进行部署。 

FA（Feature Ability）： FA支持[Page Ability](https://developer.harmonyos.com/cn/docs/documentation/doc-guides/ability-page-concept-0000000000033573)

​	Page模板是FA唯一支持的模板，用于提供与用户交互的能力。一个Page实例可以包含一组相关页面，每个页面用一个AbilitySlice实例表示。 

PA（Particle Ability）： PA支持[Service Ability](https://developer.harmonyos.com/cn/docs/documentation/doc-guides/ability-service-concept-0000000000044457)和[Data Ability](https://developer.harmonyos.com/cn/docs/documentation/doc-guides/ability-data-concept-0000000000043058) 

- Service模板：用于提供后台运行任务的能力。
- Data模板：用于对外部提供统一的数据访问抽象。

### 2.1 Page Ability

