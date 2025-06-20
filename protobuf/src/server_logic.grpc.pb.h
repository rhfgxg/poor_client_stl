// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: server_logic.proto
#ifndef GRPC_server_5flogic_2eproto__INCLUDED
#define GRPC_server_5flogic_2eproto__INCLUDED

#include "server_logic.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace rpc_server {

// 逻辑服务器
class LogicServer final {
 public:
  static constexpr char const* service_full_name() {
    return "rpc_server.LogicServer";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Process_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::rpc_server::GameActionRes* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameActionRes>> AsyncProcess_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameActionRes>>(AsyncProcess_gameActionRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameActionRes>> PrepareAsyncProcess_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameActionRes>>(PrepareAsyncProcess_gameActionRaw(context, request, cq));
    }
    // 处理游戏操作
    virtual ::grpc::Status Get_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::rpc_server::GameStateRes* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameStateRes>> AsyncGet_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameStateRes>>(AsyncGet_game_stateRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameStateRes>> PrepareAsyncGet_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameStateRes>>(PrepareAsyncGet_game_stateRaw(context, request, cq));
    }
    // 获取游戏状态
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void Process_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq* request, ::rpc_server::GameActionRes* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Process_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq* request, ::rpc_server::GameActionRes* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      // 处理游戏操作
      virtual void Get_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq* request, ::rpc_server::GameStateRes* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Get_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq* request, ::rpc_server::GameStateRes* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      // 获取游戏状态
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameActionRes>* AsyncProcess_gameActionRaw(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameActionRes>* PrepareAsyncProcess_gameActionRaw(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameStateRes>* AsyncGet_game_stateRaw(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::rpc_server::GameStateRes>* PrepareAsyncGet_game_stateRaw(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status Process_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::rpc_server::GameActionRes* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameActionRes>> AsyncProcess_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameActionRes>>(AsyncProcess_gameActionRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameActionRes>> PrepareAsyncProcess_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameActionRes>>(PrepareAsyncProcess_gameActionRaw(context, request, cq));
    }
    ::grpc::Status Get_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::rpc_server::GameStateRes* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameStateRes>> AsyncGet_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameStateRes>>(AsyncGet_game_stateRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameStateRes>> PrepareAsyncGet_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::rpc_server::GameStateRes>>(PrepareAsyncGet_game_stateRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void Process_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq* request, ::rpc_server::GameActionRes* response, std::function<void(::grpc::Status)>) override;
      void Process_gameAction(::grpc::ClientContext* context, const ::rpc_server::GameActionReq* request, ::rpc_server::GameActionRes* response, ::grpc::ClientUnaryReactor* reactor) override;
      void Get_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq* request, ::rpc_server::GameStateRes* response, std::function<void(::grpc::Status)>) override;
      void Get_game_state(::grpc::ClientContext* context, const ::rpc_server::GameStateReq* request, ::rpc_server::GameStateRes* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::rpc_server::GameActionRes>* AsyncProcess_gameActionRaw(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::rpc_server::GameActionRes>* PrepareAsyncProcess_gameActionRaw(::grpc::ClientContext* context, const ::rpc_server::GameActionReq& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::rpc_server::GameStateRes>* AsyncGet_game_stateRaw(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::rpc_server::GameStateRes>* PrepareAsyncGet_game_stateRaw(::grpc::ClientContext* context, const ::rpc_server::GameStateReq& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Process_gameAction_;
    const ::grpc::internal::RpcMethod rpcmethod_Get_game_state_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Process_gameAction(::grpc::ServerContext* context, const ::rpc_server::GameActionReq* request, ::rpc_server::GameActionRes* response);
    // 处理游戏操作
    virtual ::grpc::Status Get_game_state(::grpc::ServerContext* context, const ::rpc_server::GameStateReq* request, ::rpc_server::GameStateRes* response);
    // 获取游戏状态
  };
  template <class BaseClass>
  class WithAsyncMethod_Process_gameAction : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Process_gameAction() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Process_gameAction() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Process_gameAction(::grpc::ServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestProcess_gameAction(::grpc::ServerContext* context, ::rpc_server::GameActionReq* request, ::grpc::ServerAsyncResponseWriter< ::rpc_server::GameActionRes>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Get_game_state : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Get_game_state() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_Get_game_state() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get_game_state(::grpc::ServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGet_game_state(::grpc::ServerContext* context, ::rpc_server::GameStateReq* request, ::grpc::ServerAsyncResponseWriter< ::rpc_server::GameStateRes>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Process_gameAction<WithAsyncMethod_Get_game_state<Service > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_Process_gameAction : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Process_gameAction() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::rpc_server::GameActionReq, ::rpc_server::GameActionRes>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::rpc_server::GameActionReq* request, ::rpc_server::GameActionRes* response) { return this->Process_gameAction(context, request, response); }));}
    void SetMessageAllocatorFor_Process_gameAction(
        ::grpc::MessageAllocator< ::rpc_server::GameActionReq, ::rpc_server::GameActionRes>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::rpc_server::GameActionReq, ::rpc_server::GameActionRes>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Process_gameAction() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Process_gameAction(::grpc::ServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Process_gameAction(
      ::grpc::CallbackServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_Get_game_state : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Get_game_state() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::rpc_server::GameStateReq, ::rpc_server::GameStateRes>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::rpc_server::GameStateReq* request, ::rpc_server::GameStateRes* response) { return this->Get_game_state(context, request, response); }));}
    void SetMessageAllocatorFor_Get_game_state(
        ::grpc::MessageAllocator< ::rpc_server::GameStateReq, ::rpc_server::GameStateRes>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::rpc_server::GameStateReq, ::rpc_server::GameStateRes>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Get_game_state() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get_game_state(::grpc::ServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Get_game_state(
      ::grpc::CallbackServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_Process_gameAction<WithCallbackMethod_Get_game_state<Service > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Process_gameAction : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Process_gameAction() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Process_gameAction() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Process_gameAction(::grpc::ServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Get_game_state : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Get_game_state() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_Get_game_state() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get_game_state(::grpc::ServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Process_gameAction : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Process_gameAction() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Process_gameAction() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Process_gameAction(::grpc::ServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestProcess_gameAction(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Get_game_state : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Get_game_state() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_Get_game_state() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get_game_state(::grpc::ServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGet_game_state(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Process_gameAction : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Process_gameAction() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Process_gameAction(context, request, response); }));
    }
    ~WithRawCallbackMethod_Process_gameAction() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Process_gameAction(::grpc::ServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Process_gameAction(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Get_game_state : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Get_game_state() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Get_game_state(context, request, response); }));
    }
    ~WithRawCallbackMethod_Get_game_state() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Get_game_state(::grpc::ServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Get_game_state(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Process_gameAction : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Process_gameAction() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::rpc_server::GameActionReq, ::rpc_server::GameActionRes>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::rpc_server::GameActionReq, ::rpc_server::GameActionRes>* streamer) {
                       return this->StreamedProcess_gameAction(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Process_gameAction() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Process_gameAction(::grpc::ServerContext* /*context*/, const ::rpc_server::GameActionReq* /*request*/, ::rpc_server::GameActionRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedProcess_gameAction(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::rpc_server::GameActionReq,::rpc_server::GameActionRes>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Get_game_state : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Get_game_state() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::rpc_server::GameStateReq, ::rpc_server::GameStateRes>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::rpc_server::GameStateReq, ::rpc_server::GameStateRes>* streamer) {
                       return this->StreamedGet_game_state(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Get_game_state() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Get_game_state(::grpc::ServerContext* /*context*/, const ::rpc_server::GameStateReq* /*request*/, ::rpc_server::GameStateRes* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGet_game_state(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::rpc_server::GameStateReq,::rpc_server::GameStateRes>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Process_gameAction<WithStreamedUnaryMethod_Get_game_state<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Process_gameAction<WithStreamedUnaryMethod_Get_game_state<Service > > StreamedService;
};

}  // namespace rpc_server


#endif  // GRPC_server_5flogic_2eproto__INCLUDED
