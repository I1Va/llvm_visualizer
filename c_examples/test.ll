; ModuleID = './c_examples/test.cpp'
source_filename = "./c_examples/test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str.1 = private unnamed_addr constant [17 x i8] c"Fact(%lu) = %lu\0A\00", align 1
@str.2 = private unnamed_addr constant [34 x i8] c"Usage: 1 argument - factorial len\00", align 1
@dump_dynamic_logger_info_path = private unnamed_addr constant [22 x i8] c"info/dynamic_info.bin\00", align 1
@llvm.global_dtors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @__dtor_wrapper, ptr null }]

; Function Attrs: mustprogress nofree norecurse nosync nounwind sspstrong willreturn memory(none) uwtable
define dso_local noundef i64 @_Z4factm(i64 noundef %0) local_unnamed_addr #0 {
  call void @basic_block_start_logger(i64 94874471393312)
  %2 = icmp ult i64 %0, 2
  br i1 %2, label %45, label %3

3:                                                ; preds = %1
  call void @basic_block_start_logger(i64 94874470561264)
  %4 = add i64 %0, -1
  %5 = add i64 %0, -2
  %6 = and i64 %4, 7
  %7 = icmp ult i64 %5, 7
  br i1 %7, label %32, label %8

8:                                                ; preds = %3
  call void @basic_block_start_logger(i64 94874471745008)
  %9 = and i64 %4, -8
  br label %10

10:                                               ; preds = %10, %8
  %11 = phi i64 [ %0, %8 ], [ %28, %10 ]
  %12 = phi i64 [ 1, %8 ], [ %29, %10 ]
  %13 = phi i64 [ 0, %8 ], [ %30, %10 ]
  call void @basic_block_start_logger(i64 94874471563152)
  %14 = add i64 %11, -1
  %15 = mul i64 %11, %12
  %16 = add i64 %11, -2
  %17 = mul i64 %14, %15
  %18 = add i64 %11, -3
  %19 = mul i64 %16, %17
  %20 = add i64 %11, -4
  %21 = mul i64 %18, %19
  %22 = add i64 %11, -5
  %23 = mul i64 %20, %21
  %24 = add i64 %11, -6
  %25 = mul i64 %22, %23
  %26 = add i64 %11, -7
  %27 = mul i64 %24, %25
  %28 = add i64 %11, -8
  %29 = mul i64 %26, %27
  %30 = add i64 %13, 8
  %31 = icmp eq i64 %30, %9
  br i1 %31, label %32, label %10

32:                                               ; preds = %10, %3
  %33 = phi i64 [ poison, %3 ], [ %29, %10 ]
  %34 = phi i64 [ %0, %3 ], [ %28, %10 ]
  %35 = phi i64 [ 1, %3 ], [ %29, %10 ]
  call void @basic_block_start_logger(i64 94874470572544)
  %36 = icmp eq i64 %6, 0
  br i1 %36, label %45, label %37

37:                                               ; preds = %32, %37
  %38 = phi i64 [ %41, %37 ], [ %34, %32 ]
  %39 = phi i64 [ %42, %37 ], [ %35, %32 ]
  %40 = phi i64 [ %43, %37 ], [ 0, %32 ]
  call void @basic_block_start_logger(i64 94874470578656)
  %41 = add i64 %38, -1
  %42 = mul i64 %38, %39
  %43 = add i64 %40, 1
  %44 = icmp eq i64 %43, %6
  br i1 %44, label %45, label %37, !llvm.loop !5

45:                                               ; preds = %32, %37, %1
  %46 = phi i64 [ 1, %1 ], [ %33, %32 ], [ %42, %37 ]
  call void @basic_block_start_logger(i64 94874471563056)
  ret i64 %46
}

; Function Attrs: mustprogress norecurse nounwind sspstrong uwtable
define dso_local noundef range(i32 0, 2) i32 @main(i32 noundef %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #1 {
  call void @basic_block_start_logger(i64 94874471554496)
  %3 = icmp eq i32 %0, 2
  br i1 %3, label %7, label %4

4:                                                ; preds = %2
  call void @basic_block_start_logger(i64 94874471554592)
  call void @call_logger(i64 94874471592112, i64 94874471587416)
  %5 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  %6 = sext i32 %5 to i64
  call void @call_logger(i64 94874471565936, i64 94874470554424)
  call void @res_int_logger(i64 %6, i64 94874471592112)
  br label %68

7:                                                ; preds = %2
  call void @basic_block_start_logger(i64 94874471580832)
  call void @call_logger(i64 94874471590688, i64 94874471588680)
  %8 = tail call ptr @__errno_location() #6
  %9 = ptrtoint ptr %8 to i64
  call void @call_logger(i64 94874471498240, i64 94874470554424)
  call void @res_int_logger(i64 %9, i64 94874471590688)
  %10 = load i32, ptr %8, align 4, !tbaa !7
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %65

12:                                               ; preds = %7
  call void @basic_block_start_logger(i64 94874471588288)
  %13 = getelementptr inbounds nuw i8, ptr %1, i64 8
  %14 = load ptr, ptr %13, align 8, !tbaa !11
  call void @call_logger(i64 94874471319584, i64 94874471602168)
  %15 = tail call i64 @__isoc23_strtol(ptr noundef nonnull %14, ptr noundef null, i32 noundef 10) #7
  call void @call_logger(i64 94874471491280, i64 94874470554424)
  call void @res_int_logger(i64 %15, i64 94874471319584)
  %16 = shl i64 %15, 32
  %17 = ashr exact i64 %16, 32
  %18 = icmp ult i64 %17, 2
  br i1 %18, label %61, label %19

19:                                               ; preds = %12
  call void @basic_block_start_logger(i64 94874470557952)
  %20 = add nsw i64 %17, -1
  %21 = add nsw i64 %17, -2
  %22 = and i64 %20, 7
  %23 = icmp ult i64 %21, 7
  br i1 %23, label %48, label %24

24:                                               ; preds = %19
  call void @basic_block_start_logger(i64 94874471746816)
  %25 = and i64 %20, -8
  br label %26

26:                                               ; preds = %26, %24
  %27 = phi i64 [ %17, %24 ], [ %44, %26 ]
  %28 = phi i64 [ 1, %24 ], [ %45, %26 ]
  %29 = phi i64 [ 0, %24 ], [ %46, %26 ]
  call void @basic_block_start_logger(i64 94874470420496)
  %30 = add i64 %27, -1
  %31 = mul i64 %28, %27
  %32 = add i64 %27, -2
  %33 = mul i64 %31, %30
  %34 = add i64 %27, -3
  %35 = mul i64 %33, %32
  %36 = add i64 %27, -4
  %37 = mul i64 %35, %34
  %38 = add i64 %27, -5
  %39 = mul i64 %37, %36
  %40 = add i64 %27, -6
  %41 = mul i64 %39, %38
  %42 = add i64 %27, -7
  %43 = mul i64 %41, %40
  %44 = add i64 %27, -8
  %45 = mul i64 %43, %42
  %46 = add i64 %29, 8
  %47 = icmp eq i64 %46, %25
  br i1 %47, label %48, label %26

48:                                               ; preds = %26, %19
  %49 = phi i64 [ poison, %19 ], [ %45, %26 ]
  %50 = phi i64 [ %17, %19 ], [ %44, %26 ]
  %51 = phi i64 [ 1, %19 ], [ %45, %26 ]
  call void @basic_block_start_logger(i64 94874471746912)
  %52 = icmp eq i64 %22, 0
  br i1 %52, label %61, label %53

53:                                               ; preds = %48, %53
  %54 = phi i64 [ %57, %53 ], [ %50, %48 ]
  %55 = phi i64 [ %58, %53 ], [ %51, %48 ]
  %56 = phi i64 [ %59, %53 ], [ 0, %48 ]
  call void @basic_block_start_logger(i64 94874471562960)
  %57 = add i64 %54, -1
  %58 = mul i64 %55, %54
  %59 = add i64 %56, 1
  %60 = icmp eq i64 %59, %22
  br i1 %60, label %61, label %53, !llvm.loop !14

61:                                               ; preds = %48, %53, %12
  %62 = phi i64 [ 1, %12 ], [ %49, %48 ], [ %58, %53 ]
  call void @basic_block_start_logger(i64 94874470575616)
  call void @call_logger(i64 94874470058448, i64 94874471581624)
  %63 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1, i64 noundef %17, i64 noundef %62)
  %64 = sext i32 %63 to i64
  call void @call_logger(i64 94874471494256, i64 94874470554424)
  call void @res_int_logger(i64 %64, i64 94874470058448)
  br label %68

65:                                               ; preds = %7
  call void @basic_block_start_logger(i64 94874471588480)
  call void @call_logger(i64 94874471596160, i64 94874471587416)
  %66 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  %67 = sext i32 %66 to i64
  call void @call_logger(i64 94874471495024, i64 94874470554424)
  call void @res_int_logger(i64 %67, i64 94874471596160)
  br label %68

68:                                               ; preds = %65, %61, %4
  %69 = phi i32 [ 1, %4 ], [ 0, %61 ], [ 1, %65 ]
  call void @basic_block_start_logger(i64 94874470735984)
  ret i32 %69
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: mustprogress nofree nosync nounwind willreturn memory(none)
declare ptr @__errno_location() local_unnamed_addr #3

; Function Attrs: nounwind
declare i64 @__isoc23_strtol(ptr noundef, ptr noundef, i32 noundef) local_unnamed_addr #4

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #5

declare void @call_logger(i64, i64)

declare void @res_int_logger(i64, i64)

declare void @basic_block_start_logger(i64)

define internal void @__dtor_wrapper() {
  call void @dump_dynamic_logger_info(ptr @dump_dynamic_logger_info_path)
  ret void
}

declare void @dump_dynamic_logger_info(ptr)

attributes #0 = { mustprogress nofree norecurse nosync nounwind sspstrong willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress norecurse nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress nofree nosync nounwind willreturn memory(none) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nofree nounwind }
attributes #6 = { nounwind willreturn memory(none) }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 21.1.6"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.unroll.disable"}
!7 = !{!8, !8, i64 0}
!8 = !{!"int", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C++ TBAA"}
!11 = !{!12, !12, i64 0}
!12 = !{!"p1 omnipotent char", !13, i64 0}
!13 = !{!"any pointer", !9, i64 0}
!14 = distinct !{!14, !6}
