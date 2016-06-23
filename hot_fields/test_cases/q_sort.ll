; ModuleID = 'input.c'
source_filename = "input.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.11.0"

%struct.student = type { [10 x i8], [10 x i8], float }

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c"%s \00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %record = alloca %struct.student*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* %n)
  %0 = load i32, i32* %n, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 24
  %call1 = call i8* @malloc(i64 %mul)
  %1 = bitcast i8* %call1 to %struct.student*
  store %struct.student* %1, %struct.student** %record, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %2 = load i32, i32* %i, align 4
  %3 = load i32, i32* %n, align 4
  %cmp = icmp slt i32 %2, %3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %4 = load %struct.student*, %struct.student** %record, align 8
  %5 = load i32, i32* %i, align 4
  %idx.ext = sext i32 %5 to i64
  %add.ptr = getelementptr inbounds %struct.student, %struct.student* %4, i64 %idx.ext
  %usn = getelementptr inbounds %struct.student, %struct.student* %add.ptr, i32 0, i32 0
  %arraydecay = getelementptr inbounds [10 x i8], [10 x i8]* %usn, i32 0, i32 0
  %call3 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i8* %arraydecay)
  %6 = load %struct.student*, %struct.student** %record, align 8
  %7 = load i32, i32* %i, align 4
  %idx.ext4 = sext i32 %7 to i64
  %add.ptr5 = getelementptr inbounds %struct.student, %struct.student* %6, i64 %idx.ext4
  %name = getelementptr inbounds %struct.student, %struct.student* %add.ptr5, i32 0, i32 1
  %arraydecay6 = getelementptr inbounds [10 x i8], [10 x i8]* %name, i32 0, i32 0
  %call7 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i8* %arraydecay6)
  %8 = load %struct.student*, %struct.student** %record, align 8
  %9 = load i32, i32* %i, align 4
  %idx.ext8 = sext i32 %9 to i64
  %add.ptr9 = getelementptr inbounds %struct.student, %struct.student* %8, i64 %idx.ext8
  %gpa = getelementptr inbounds %struct.student, %struct.student* %add.ptr9, i32 0, i32 2
  %call10 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), float* %gpa)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %10 = load i32, i32* %i, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %11 = load %struct.student*, %struct.student** %record, align 8
  %12 = load i32, i32* %n, align 4
  %sub = sub nsw i32 %12, 1
  call void @quick_sort(%struct.student* %11, i32 0, i32 %sub)
  %13 = load %struct.student*, %struct.student** %record, align 8
  %14 = load i32, i32* %n, align 4
  call void @display(%struct.student* %13, i32 %14)
  %15 = load %struct.student*, %struct.student** %record, align 8
  %16 = bitcast %struct.student* %15 to i8*
  call void @free(i8* %16)
  %17 = load i32, i32* %retval, align 4
  ret i32 %17
}

declare i32 @scanf(i8*, ...) #1

declare i8* @malloc(i64) #1

; Function Attrs: nounwind ssp uwtable
define void @quick_sort(%struct.student* %record, i32 %start, i32 %stop) #0 {
entry:
  %record.addr = alloca %struct.student*, align 8
  %start.addr = alloca i32, align 4
  %stop.addr = alloca i32, align 4
  %middle = alloca i32, align 4
  store %struct.student* %record, %struct.student** %record.addr, align 8
  store i32 %start, i32* %start.addr, align 4
  store i32 %stop, i32* %stop.addr, align 4
  %0 = load i32, i32* %start.addr, align 4
  %1 = load i32, i32* %stop.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %2 = load %struct.student*, %struct.student** %record.addr, align 8
  %3 = load i32, i32* %start.addr, align 4
  %4 = load i32, i32* %stop.addr, align 4
  %call = call i32 @partition(%struct.student* %2, i32 %3, i32 %4)
  store i32 %call, i32* %middle, align 4
  %5 = load %struct.student*, %struct.student** %record.addr, align 8
  %6 = load i32, i32* %start.addr, align 4
  %7 = load i32, i32* %middle, align 4
  %sub = sub nsw i32 %7, 1
  call void @quick_sort(%struct.student* %5, i32 %6, i32 %sub)
  %8 = load %struct.student*, %struct.student** %record.addr, align 8
  %9 = load i32, i32* %middle, align 4
  %add = add nsw i32 %9, 1
  %10 = load i32, i32* %stop.addr, align 4
  call void @quick_sort(%struct.student* %8, i32 %add, i32 %10)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @display(%struct.student* %record, i32 %n) #0 {
entry:
  %record.addr = alloca %struct.student*, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store %struct.student* %record, %struct.student** %record.addr, align 8
  store i32 %n, i32* %n.addr, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load %struct.student*, %struct.student** %record.addr, align 8
  %3 = load i32, i32* %i, align 4
  %idx.ext = sext i32 %3 to i64
  %add.ptr = getelementptr inbounds %struct.student, %struct.student* %2, i64 %idx.ext
  %usn = getelementptr inbounds %struct.student, %struct.student* %add.ptr, i32 0, i32 0
  %arraydecay = getelementptr inbounds [10 x i8], [10 x i8]* %usn, i32 0, i32 0
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i32 0, i32 0), i8* %arraydecay)
  %4 = load %struct.student*, %struct.student** %record.addr, align 8
  %5 = load i32, i32* %i, align 4
  %idx.ext1 = sext i32 %5 to i64
  %add.ptr2 = getelementptr inbounds %struct.student, %struct.student* %4, i64 %idx.ext1
  %name = getelementptr inbounds %struct.student, %struct.student* %add.ptr2, i32 0, i32 1
  %arraydecay3 = getelementptr inbounds [10 x i8], [10 x i8]* %name, i32 0, i32 0
  %call4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i32 0, i32 0), i8* %arraydecay3)
  %6 = load %struct.student*, %struct.student** %record.addr, align 8
  %7 = load i32, i32* %i, align 4
  %idx.ext5 = sext i32 %7 to i64
  %add.ptr6 = getelementptr inbounds %struct.student, %struct.student* %6, i64 %idx.ext5
  %gpa = getelementptr inbounds %struct.student, %struct.student* %add.ptr6, i32 0, i32 2
  %8 = load float, float* %gpa, align 4
  %conv = fpext float %8 to double
  %call7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.4, i32 0, i32 0), double %conv)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %9 = load i32, i32* %i, align 4
  %inc = add nsw i32 %9, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

declare void @free(i8*) #1

; Function Attrs: nounwind ssp uwtable
define i32 @partition(%struct.student* %record, i32 %start, i32 %stop) #0 {
entry:
  %record.addr = alloca %struct.student*, align 8
  %start.addr = alloca i32, align 4
  %stop.addr = alloca i32, align 4
  %pivot = alloca %struct.student*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %temp = alloca %struct.student, align 4
  %temp13 = alloca %struct.student, align 4
  store %struct.student* %record, %struct.student** %record.addr, align 8
  store i32 %start, i32* %start.addr, align 4
  store i32 %stop, i32* %stop.addr, align 4
  %0 = load %struct.student*, %struct.student** %record.addr, align 8
  %1 = load i32, i32* %stop.addr, align 4
  %idx.ext = sext i32 %1 to i64
  %add.ptr = getelementptr inbounds %struct.student, %struct.student* %0, i64 %idx.ext
  store %struct.student* %add.ptr, %struct.student** %pivot, align 8
  %2 = load i32, i32* %start.addr, align 4
  %sub = sub nsw i32 %2, 1
  store i32 %sub, i32* %i, align 4
  %3 = load i32, i32* %start.addr, align 4
  store i32 %3, i32* %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %4 = load i32, i32* %j, align 4
  %5 = load i32, i32* %stop.addr, align 4
  %cmp = icmp slt i32 %4, %5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %6 = load %struct.student*, %struct.student** %record.addr, align 8
  %7 = load i32, i32* %j, align 4
  %idx.ext1 = sext i32 %7 to i64
  %add.ptr2 = getelementptr inbounds %struct.student, %struct.student* %6, i64 %idx.ext1
  %8 = load %struct.student*, %struct.student** %pivot, align 8
  %call = call i32 @compare(%struct.student* %add.ptr2, %struct.student* %8)
  %cmp3 = icmp eq i32 %call, 1
  br i1 %cmp3, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %9 = load i32, i32* %i, align 4
  %inc = add nsw i32 %9, 1
  store i32 %inc, i32* %i, align 4
  %10 = load %struct.student*, %struct.student** %record.addr, align 8
  %11 = load i32, i32* %j, align 4
  %idx.ext4 = sext i32 %11 to i64
  %add.ptr5 = getelementptr inbounds %struct.student, %struct.student* %10, i64 %idx.ext4
  %12 = bitcast %struct.student* %temp to i8*
  %13 = bitcast %struct.student* %add.ptr5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %12, i8* %13, i64 24, i32 4, i1 false)
  %14 = load %struct.student*, %struct.student** %record.addr, align 8
  %15 = load i32, i32* %j, align 4
  %idx.ext6 = sext i32 %15 to i64
  %add.ptr7 = getelementptr inbounds %struct.student, %struct.student* %14, i64 %idx.ext6
  %16 = load %struct.student*, %struct.student** %record.addr, align 8
  %17 = load i32, i32* %i, align 4
  %idx.ext8 = sext i32 %17 to i64
  %add.ptr9 = getelementptr inbounds %struct.student, %struct.student* %16, i64 %idx.ext8
  %18 = bitcast %struct.student* %add.ptr7 to i8*
  %19 = bitcast %struct.student* %add.ptr9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %18, i8* %19, i64 24, i32 4, i1 false)
  %20 = load %struct.student*, %struct.student** %record.addr, align 8
  %21 = load i32, i32* %i, align 4
  %idx.ext10 = sext i32 %21 to i64
  %add.ptr11 = getelementptr inbounds %struct.student, %struct.student* %20, i64 %idx.ext10
  %22 = bitcast %struct.student* %add.ptr11 to i8*
  %23 = bitcast %struct.student* %temp to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %22, i8* %23, i64 24, i32 4, i1 false)
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %24 = load i32, i32* %j, align 4
  %inc12 = add nsw i32 %24, 1
  store i32 %inc12, i32* %j, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %25 = load %struct.student*, %struct.student** %record.addr, align 8
  %26 = load i32, i32* %stop.addr, align 4
  %idx.ext14 = sext i32 %26 to i64
  %add.ptr15 = getelementptr inbounds %struct.student, %struct.student* %25, i64 %idx.ext14
  %27 = bitcast %struct.student* %temp13 to i8*
  %28 = bitcast %struct.student* %add.ptr15 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 24, i32 4, i1 false)
  %29 = load %struct.student*, %struct.student** %record.addr, align 8
  %30 = load i32, i32* %stop.addr, align 4
  %idx.ext16 = sext i32 %30 to i64
  %add.ptr17 = getelementptr inbounds %struct.student, %struct.student* %29, i64 %idx.ext16
  %31 = load %struct.student*, %struct.student** %record.addr, align 8
  %32 = load i32, i32* %i, align 4
  %idx.ext18 = sext i32 %32 to i64
  %add.ptr19 = getelementptr inbounds %struct.student, %struct.student* %31, i64 %idx.ext18
  %add.ptr20 = getelementptr inbounds %struct.student, %struct.student* %add.ptr19, i64 1
  %33 = bitcast %struct.student* %add.ptr17 to i8*
  %34 = bitcast %struct.student* %add.ptr20 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %33, i8* %34, i64 24, i32 4, i1 false)
  %35 = load %struct.student*, %struct.student** %record.addr, align 8
  %36 = load i32, i32* %i, align 4
  %idx.ext21 = sext i32 %36 to i64
  %add.ptr22 = getelementptr inbounds %struct.student, %struct.student* %35, i64 %idx.ext21
  %add.ptr23 = getelementptr inbounds %struct.student, %struct.student* %add.ptr22, i64 1
  %37 = bitcast %struct.student* %add.ptr23 to i8*
  %38 = bitcast %struct.student* %temp13 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %37, i8* %38, i64 24, i32 4, i1 false)
  %39 = load i32, i32* %i, align 4
  %add = add nsw i32 %39, 1
  ret i32 %add
}

; Function Attrs: nounwind ssp uwtable
define i32 @compare(%struct.student* %a, %struct.student* %b) #0 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca %struct.student*, align 8
  %b.addr = alloca %struct.student*, align 8
  %v = alloca i32, align 4
  store %struct.student* %a, %struct.student** %a.addr, align 8
  store %struct.student* %b, %struct.student** %b.addr, align 8
  %0 = load %struct.student*, %struct.student** %a.addr, align 8
  %name = getelementptr inbounds %struct.student, %struct.student* %0, i32 0, i32 1
  %arraydecay = getelementptr inbounds [10 x i8], [10 x i8]* %name, i32 0, i32 0
  %1 = load %struct.student*, %struct.student** %b.addr, align 8
  %name1 = getelementptr inbounds %struct.student, %struct.student* %1, i32 0, i32 1
  %arraydecay2 = getelementptr inbounds [10 x i8], [10 x i8]* %name1, i32 0, i32 0
  %call = call i32 @strcmp(i8* %arraydecay, i8* %arraydecay2)
  store i32 %call, i32* %v, align 4
  %2 = load i32, i32* %v, align 4
  %cmp = icmp slt i32 %2, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 1, i32* %retval, align 4
  br label %return

if.else:                                          ; preds = %entry
  store i32 0, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.else, %if.then
  %3 = load i32, i32* %retval, align 4
  ret i32 %3
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

declare i32 @strcmp(i8*, i8*) #1

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind ssp uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"clang version 3.9.0 (http://llvm.org/git/clang.git 44a5e636a46ebaf333fc9e9e18863f67bbb1a1ed) (http://llvm.org/git/llvm.git ff7edbe38c28426980827e63ba2e528fd44da1e1)"}
