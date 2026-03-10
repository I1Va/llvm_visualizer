; ModuleID = 'c_examples/front.cpp'
source_filename = "c_examples/front.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.lexem_t = type { i32, %union.token_value_t, %struct.text_pos_t, i64, i8 }
%union.token_value_t = type { i64 }
%struct.text_pos_t = type { i64, i64 }

@.str = private unnamed_addr constant [5 x i8] c"data\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"./src/lang_grammar.cpp\00", align 1
@__PRETTY_FUNCTION__._Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E = private unnamed_addr constant [87 x i8] c"ast_tree_elem_t *try_grule(parsing_block_t *, ast_tree_elem_t *(*)(parsing_block_t *))\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"grule_func\00", align 1
@__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num = private unnamed_addr constant [73 x i8] c"void start_parser_err(parsing_block_t *, lexem_t, enum grammar_rule_num)\00", align 1
@stdout = external local_unnamed_addr global ptr, align 8
@.str.3 = private unnamed_addr constant [28 x i8] c"!data->parser_err.err_state\00", align 1
@.str.4 = private unnamed_addr constant [47 x i8] c"data->parser_err.lex.token_type == TOKEN_EMPTY\00", align 1
@.str.5 = private unnamed_addr constant [34 x i8] c"!data->parser_err.grule_list_size\00", align 1
@.str.6 = private unnamed_addr constant [11 x i8] c"parser_err\00", align 1
@__PRETTY_FUNCTION__._Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num = private unnamed_addr constant [68 x i8] c"void add_grule_to_parser_err(parser_err_t *, enum grammar_rule_num)\00", align 1
@.str.7 = private unnamed_addr constant [22 x i8] c"parser_err->err_state\00", align 1
@__PRETTY_FUNCTION__._Z16clear_parser_errP12parser_err_t = private unnamed_addr constant [38 x i8] c"void clear_parser_err(parser_err_t *)\00", align 1
@.str.8 = private unnamed_addr constant [7 x i8] c"stream\00", align 1
@__PRETTY_FUNCTION__._Z16check_parser_errP8_IO_FILEP15parsing_block_t = private unnamed_addr constant [49 x i8] c"bool check_parser_err(FILE *, parsing_block_t *)\00", align 1
@.str.9 = private unnamed_addr constant [13 x i8] c"SYNTAX_ERROR\00", align 1
@.str.10 = private unnamed_addr constant [14 x i8] c"ERROR_LEXEM: \00", align 1
@.str.12 = private unnamed_addr constant [14 x i8] c"ERROR_PATH: \0A\00", align 1
@__PRETTY_FUNCTION__._Z13dump_last_lexP15parsing_block_t = private unnamed_addr constant [38 x i8] c"void dump_last_lex(parsing_block_t *)\00", align 1
@.str.13 = private unnamed_addr constant [10 x i8] c"DEBUG_LEX\00", align 1

; Function Attrs: mustprogress sspstrong uwtable
define dso_local noundef ptr @_Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E(ptr noundef %0, ptr noundef readonly captures(address_is_null) %1) local_unnamed_addr #0 {
  %3 = icmp eq ptr %0, null
  br i1 %3, label %4, label %5

4:                                                ; preds = %2
  tail call void @__assert_fail(ptr noundef nonnull @.str, ptr noundef nonnull @.str.1, i32 noundef 105, ptr noundef nonnull @__PRETTY_FUNCTION__._Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E) #7
  unreachable

5:                                                ; preds = %2
  %6 = icmp eq ptr %1, null
  br i1 %6, label %7, label %8

7:                                                ; preds = %5
  tail call void @__assert_fail(ptr noundef nonnull @.str.2, ptr noundef nonnull @.str.1, i32 noundef 106, ptr noundef nonnull @__PRETTY_FUNCTION__._Z9try_gruleP15parsing_block_tPFP15ast_tree_elem_tS0_E) #7
  unreachable

8:                                                ; preds = %5
  %9 = getelementptr inbounds nuw i8, ptr %0, i64 56
  %10 = load i64, ptr %9, align 8, !tbaa !5
  %11 = tail call noundef ptr %1(ptr noundef nonnull %0)
  %12 = getelementptr inbounds nuw i8, ptr %0, i64 72
  %13 = load i8, ptr %12, align 8, !tbaa !23, !range !24, !noundef !25
  %14 = trunc nuw i8 %13 to i1
  br i1 %14, label %15, label %18

15:                                               ; preds = %8
  %16 = getelementptr inbounds nuw i8, ptr %0, i64 640
  store i64 0, ptr %16, align 8, !tbaa !26
  store i8 0, ptr %12, align 8, !tbaa !27
  %17 = getelementptr inbounds nuw i8, ptr %0, i64 80
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(52) %17, i8 0, i64 52, i1 false)
  store i64 %10, ptr %9, align 8, !tbaa !5
  br label %18

18:                                               ; preds = %8, %15
  %19 = phi ptr [ null, %15 ], [ %11, %8 ]
  ret ptr %19
}

; Function Attrs: cold noreturn nounwind
declare void @__assert_fail(ptr noundef, ptr noundef, i32 noundef, ptr noundef) local_unnamed_addr #1

; Function Attrs: mustprogress nounwind sspstrong uwtable
define dso_local void @_Z16clear_parser_errP12parser_err_t(ptr noundef captures(address_is_null) %0) local_unnamed_addr #2 {
  %2 = icmp eq ptr %0, null
  br i1 %2, label %3, label %4

3:                                                ; preds = %1
  tail call void @__assert_fail(ptr noundef nonnull @.str.6, ptr noundef nonnull @.str.1, i32 noundef 144, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16clear_parser_errP12parser_err_t) #7
  unreachable

4:                                                ; preds = %1
  %5 = load i8, ptr %0, align 8, !tbaa !27, !range !24, !noundef !25
  %6 = trunc nuw i8 %5 to i1
  br i1 %6, label %8, label %7

7:                                                ; preds = %4
  tail call void @__assert_fail(ptr noundef nonnull @.str.7, ptr noundef nonnull @.str.1, i32 noundef 145, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16clear_parser_errP12parser_err_t) #7
  unreachable

8:                                                ; preds = %4
  %9 = getelementptr inbounds nuw i8, ptr %0, i64 568
  store i64 0, ptr %9, align 8, !tbaa !26
  %10 = getelementptr inbounds nuw i8, ptr %0, i64 56
  store i32 0, ptr %10, align 8, !tbaa !28
  store i8 0, ptr %0, align 8, !tbaa !27
  %11 = getelementptr inbounds nuw i8, ptr %0, i64 8
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %11, i8 0, i64 48, i1 false)
  store i32 0, ptr %11, align 8, !tbaa !30
  ret void
}

; Function Attrs: mustprogress sspstrong uwtable
define dso_local void @_Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num(ptr noundef %0, ptr noundef readonly byval(%struct.lexem_t) align 8 captures(none) %1, i32 noundef %2) local_unnamed_addr #0 {
  %4 = icmp eq ptr %0, null
  br i1 %4, label %5, label %6

5:                                                ; preds = %3
  tail call void @__assert_fail(ptr noundef nonnull @.str, ptr noundef nonnull @.str.1, i32 noundef 121, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

6:                                                ; preds = %3
  %7 = getelementptr inbounds nuw i8, ptr %0, i64 72
  %8 = load i8, ptr %7, align 8, !tbaa !23, !range !24, !noundef !25
  %9 = trunc nuw i8 %8 to i1
  br i1 %9, label %10, label %16

10:                                               ; preds = %6
  %11 = load ptr, ptr @stdout, align 8, !tbaa !31
  %12 = tail call noundef zeroext i1 @_Z16check_parser_errP8_IO_FILEP15parsing_block_t(ptr noundef %11, ptr noundef nonnull %0)
  %13 = load i8, ptr %7, align 8, !tbaa !23, !range !24, !noundef !25
  %14 = trunc nuw i8 %13 to i1
  br i1 %14, label %15, label %16

15:                                               ; preds = %10
  tail call void @__assert_fail(ptr noundef nonnull @.str.3, ptr noundef nonnull @.str.1, i32 noundef 124, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

16:                                               ; preds = %10, %6
  %17 = getelementptr inbounds nuw i8, ptr %0, i64 80
  %18 = load i32, ptr %17, align 8, !tbaa !32
  %19 = icmp eq i32 %18, 0
  br i1 %19, label %21, label %20

20:                                               ; preds = %16
  tail call void @__assert_fail(ptr noundef nonnull @.str.4, ptr noundef nonnull @.str.1, i32 noundef 127, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

21:                                               ; preds = %16
  %22 = getelementptr inbounds nuw i8, ptr %0, i64 640
  %23 = load i64, ptr %22, align 8, !tbaa !33
  %24 = icmp eq i64 %23, 0
  br i1 %24, label %26, label %25

25:                                               ; preds = %21
  tail call void @__assert_fail(ptr noundef nonnull @.str.5, ptr noundef nonnull @.str.1, i32 noundef 128, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16start_parser_errP15parsing_block_t7lexem_t16grammar_rule_num) #7
  unreachable

26:                                               ; preds = %21
  store i8 1, ptr %7, align 8, !tbaa !23
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %17, ptr noundef nonnull align 8 dereferenceable(48) %1, i64 48, i1 false), !tbaa.struct !34
  %27 = getelementptr inbounds nuw i8, ptr %0, i64 128
  store i32 %2, ptr %27, align 8, !tbaa !28
  store i64 1, ptr %22, align 8, !tbaa !33
  ret void
}

; Function Attrs: mustprogress sspstrong uwtable
define dso_local noundef zeroext i1 @_Z16check_parser_errP8_IO_FILEP15parsing_block_t(ptr noundef %0, ptr noundef %1) local_unnamed_addr #0 {
  %3 = icmp eq ptr %0, null
  br i1 %3, label %4, label %5

4:                                                ; preds = %2
  tail call void @__assert_fail(ptr noundef nonnull @.str.8, ptr noundef nonnull @.str.1, i32 noundef 155, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16check_parser_errP8_IO_FILEP15parsing_block_t) #7
  unreachable

5:                                                ; preds = %2
  %6 = icmp eq ptr %1, null
  br i1 %6, label %7, label %8

7:                                                ; preds = %5
  tail call void @__assert_fail(ptr noundef nonnull @.str, ptr noundef nonnull @.str.1, i32 noundef 156, ptr noundef nonnull @__PRETTY_FUNCTION__._Z16check_parser_errP8_IO_FILEP15parsing_block_t) #7
  unreachable

8:                                                ; preds = %5
  %9 = getelementptr inbounds nuw i8, ptr %1, i64 72
  %10 = load i8, ptr %9, align 8, !tbaa !23, !range !24, !noundef !25
  %11 = trunc nuw i8 %10 to i1
  br i1 %11, label %12, label %32

12:                                               ; preds = %8
  tail call void @_Z13fprintf_titleP8_IO_FILEPKccm(ptr noundef nonnull %0, ptr noundef nonnull @.str.9, i8 noundef signext 45, i64 noundef 100)
  %13 = tail call i64 @fwrite(ptr nonnull @.str.10, i64 13, i64 1, ptr nonnull %0)
  %14 = getelementptr inbounds nuw i8, ptr %1, i64 80
  tail call void @_Z10lexem_dumpP8_IO_FILEP15parsing_block_t7lexem_t(ptr noundef nonnull %0, ptr noundef nonnull %1, ptr noundef nonnull byval(%struct.lexem_t) align 8 %14)
  %15 = tail call i32 @putchar(i32 10)
  %16 = tail call i64 @fwrite(ptr nonnull @.str.12, i64 13, i64 1, ptr nonnull %0)
  %17 = getelementptr inbounds nuw i8, ptr %1, i64 640
  %18 = load i64, ptr %17, align 8, !tbaa !33
  %19 = icmp eq i64 %18, 0
  br i1 %19, label %22, label %20

20:                                               ; preds = %12
  %21 = getelementptr inbounds nuw i8, ptr %1, i64 128
  br label %24

22:                                               ; preds = %24, %12
  %23 = tail call i32 @fputc(i32 10, ptr nonnull %0)
  tail call void @_Z14fprintf_borderP8_IO_FILEcmb(ptr noundef nonnull %0, i8 noundef signext 45, i64 noundef 100, i1 noundef zeroext true)
  br label %32

24:                                               ; preds = %20, %24
  %25 = phi i64 [ 0, %20 ], [ %29, %24 ]
  %26 = getelementptr inbounds nuw [128 x i32], ptr %21, i64 0, i64 %25
  %27 = load i32, ptr %26, align 4, !tbaa !28
  tail call void @_Z10grule_dumpP8_IO_FILE16grammar_rule_num(ptr noundef nonnull %0, i32 noundef %27)
  %28 = tail call i32 @fputc(i32 10, ptr nonnull %0)
  %29 = add nuw i64 %25, 1
  %30 = load i64, ptr %17, align 8, !tbaa !33
  %31 = icmp ult i64 %29, %30
  br i1 %31, label %24, label %22, !llvm.loop !39

32:                                               ; preds = %8, %22
  ret i1 %11
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #3

; Function Attrs: mustprogress nounwind sspstrong uwtable
define dso_local void @_Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num(ptr noundef captures(address_is_null) %0, i32 noundef %1) local_unnamed_addr #2 {
  %3 = icmp eq ptr %0, null
  br i1 %3, label %4, label %5

4:                                                ; preds = %2
  tail call void @__assert_fail(ptr noundef nonnull @.str.6, ptr noundef nonnull @.str.1, i32 noundef 137, ptr noundef nonnull @__PRETTY_FUNCTION__._Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num) #7
  unreachable

5:                                                ; preds = %2
  %6 = load i8, ptr %0, align 8, !tbaa !27, !range !24, !noundef !25
  %7 = trunc nuw i8 %6 to i1
  br i1 %7, label %9, label %8

8:                                                ; preds = %5
  tail call void @__assert_fail(ptr noundef nonnull @.str.7, ptr noundef nonnull @.str.1, i32 noundef 138, ptr noundef nonnull @__PRETTY_FUNCTION__._Z23add_grule_to_parser_errP12parser_err_t16grammar_rule_num) #7
  unreachable

9:                                                ; preds = %5
  %10 = getelementptr inbounds nuw i8, ptr %0, i64 56
  %11 = getelementptr inbounds nuw i8, ptr %0, i64 568
  %12 = load i64, ptr %11, align 8, !tbaa !26
  %13 = add i64 %12, 1
  store i64 %13, ptr %11, align 8, !tbaa !26
  %14 = getelementptr inbounds nuw [128 x i32], ptr %10, i64 0, i64 %12
  store i32 %1, ptr %14, align 4, !tbaa !28
  ret void
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #4

declare void @_Z13fprintf_titleP8_IO_FILEPKccm(ptr noundef, ptr noundef, i8 noundef signext, i64 noundef) local_unnamed_addr #5

declare void @_Z10lexem_dumpP8_IO_FILEP15parsing_block_t7lexem_t(ptr noundef, ptr noundef, ptr noundef byval(%struct.lexem_t) align 8) local_unnamed_addr #5

declare void @_Z10grule_dumpP8_IO_FILE16grammar_rule_num(ptr noundef, i32 noundef) local_unnamed_addr #5

declare void @_Z14fprintf_borderP8_IO_FILEcmb(ptr noundef, i8 noundef signext, i64 noundef, i1 noundef zeroext) local_unnamed_addr #5

; Function Attrs: mustprogress sspstrong uwtable
define dso_local void @_Z13dump_last_lexP15parsing_block_t(ptr noundef %0) local_unnamed_addr #0 {
  %2 = icmp eq ptr %0, null
  br i1 %2, label %3, label %4

3:                                                ; preds = %1
  tail call void @__assert_fail(ptr noundef nonnull @.str, ptr noundef nonnull @.str.1, i32 noundef 181, ptr noundef nonnull @__PRETTY_FUNCTION__._Z13dump_last_lexP15parsing_block_t) #7
  unreachable

4:                                                ; preds = %1
  %5 = load ptr, ptr @stdout, align 8, !tbaa !31
  tail call void @_Z13fprintf_titleP8_IO_FILEPKccm(ptr noundef %5, ptr noundef nonnull @.str.13, i8 noundef signext 45, i64 noundef 100)
  %6 = load ptr, ptr @stdout, align 8, !tbaa !31
  %7 = getelementptr inbounds nuw i8, ptr %0, i64 48
  %8 = load ptr, ptr %7, align 8, !tbaa !41
  %9 = getelementptr inbounds nuw i8, ptr %0, i64 56
  %10 = load i64, ptr %9, align 8, !tbaa !5
  %11 = getelementptr inbounds nuw %struct.lexem_t, ptr %8, i64 %10
  tail call void @_Z10lexem_dumpP8_IO_FILEP15parsing_block_t7lexem_t(ptr noundef %6, ptr noundef nonnull %0, ptr noundef nonnull byval(%struct.lexem_t) align 8 %11)
  %12 = tail call i32 @putchar(i32 10)
  %13 = load ptr, ptr @stdout, align 8, !tbaa !31
  tail call void @_Z14fprintf_borderP8_IO_FILEcmb(ptr noundef %13, i8 noundef signext 45, i64 noundef 100, i1 noundef zeroext true)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #6

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #6

; Function Attrs: nofree nounwind
declare noundef i32 @fputc(i32 noundef, ptr noundef captures(none)) local_unnamed_addr #6

attributes #0 = { mustprogress sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { cold noreturn nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nofree nounwind }
attributes #7 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 21.1.6"}
!5 = !{!6, !11, i64 56}
!6 = !{!"_ZTS15parsing_block_t", !7, i64 0, !11, i64 8, !12, i64 16, !11, i64 24, !13, i64 32, !11, i64 40, !14, i64 48, !11, i64 56, !11, i64 64, !15, i64 72, !20, i64 648, !22, i64 656}
!7 = !{!"p1 omnipotent char", !8, i64 0}
!8 = !{!"any pointer", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C++ TBAA"}
!11 = !{!"long", !9, i64 0}
!12 = !{!"p1 _ZTS9keyword_t", !8, i64 0}
!13 = !{!"p1 _ZTS6name_t", !8, i64 0}
!14 = !{!"p1 _ZTS7lexem_t", !8, i64 0}
!15 = !{!"_ZTS12parser_err_t", !16, i64 0, !17, i64 8, !9, i64 56, !11, i64 568}
!16 = !{!"bool", !9, i64 0}
!17 = !{!"_ZTS7lexem_t", !18, i64 0, !9, i64 8, !19, i64 16, !11, i64 32, !16, i64 40}
!18 = !{!"_ZTS13lexer_token_t", !9, i64 0}
!19 = !{!"_ZTS10text_pos_t", !11, i64 0, !11, i64 8}
!20 = !{!"p2 _ZTS13str_storage_t", !21, i64 0}
!21 = !{!"any p2 pointer", !8, i64 0}
!22 = !{!"p1 _ZTS8_IO_FILE", !8, i64 0}
!23 = !{!6, !16, i64 72}
!24 = !{i8 0, i8 2}
!25 = !{}
!26 = !{!15, !11, i64 568}
!27 = !{!15, !16, i64 0}
!28 = !{!29, !29, i64 0}
!29 = !{!"_ZTS16grammar_rule_num", !9, i64 0}
!30 = !{!15, !18, i64 8}
!31 = !{!22, !22, i64 0}
!32 = !{!6, !18, i64 80}
!33 = !{!6, !11, i64 640}
!34 = !{i64 0, i64 4, !35, i64 8, i64 8, !36, i64 16, i64 8, !37, i64 24, i64 8, !37, i64 32, i64 8, !37, i64 40, i64 1, !38}
!35 = !{!18, !18, i64 0}
!36 = !{!9, !9, i64 0}
!37 = !{!11, !11, i64 0}
!38 = !{!16, !16, i64 0}
!39 = distinct !{!39, !40}
!40 = !{!"llvm.loop.mustprogress"}
!41 = !{!6, !14, i64 48}
