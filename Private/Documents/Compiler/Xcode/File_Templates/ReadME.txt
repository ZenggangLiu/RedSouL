
---------------------- 如何创建 TEMPLATES ----------------------------
1. 在 ~/Library/Developer/Xcode/Templates 下面创建一个目录：例如叫做 RedSoul

2. 将Xcode预先提供的 File Templates（参加如下的路径） 拷贝到创建的目录下.
   /Applications/Xcode.app/Contents/Developer/Library/Xcode/Templates/File Templates/Source

3. 我们在创建 Template 的时候可以使用如下宏：

__FILENAME___ : 当前的带文件扩展的文件名

___FILEBASENAMEASIDENTIFIER___ : 当前文件名（无扩展名）：converted to valid C-style identifier

___PROJECTNAME___ : 项目名称

___PROJECTNAMEASIDENTIFIER___ : 项目名称： converted to valid C-style identifier

___USERNAME___ : 短用户账号名。例如 “Bob”

___FULLUSERNAME___ : 完全用户名。 例如 “Bob McCune”

___ORGANIZATIONNAME___ : 组织名： the organization name

___DATE___ : 今日的日期:  6/9/12

___TIME___ : 当前时间: 5:09 PM

___YEAR___ : 当前年份（四位）: 2012



----------------------- 如何安装 Templates ----------------------------
1. 解压RedSoul.zip
2. 将整个目录拷贝到 ~/Library/Developer/Xcode/Templates


----------------------- 如何使用 Templates -------------------------------
1. 旋转 “New File”， 在 RedSoUL template 列表下选择文件类型
