Rails.application.routes.draw do
  
  get "home/response"
  post "submit_date", to: "home#submit_date"

  root "home#index"
end
